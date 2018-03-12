#include <gtest/gtest.h>
#include <inc/matrix4x4.hpp>
#include "matrix.hpp"

TEST(Decomposition, LUNoPivoting)
{
    // https://en.wikipedia.org/wiki/LU_decomposition

    double m_data[4] = {4.0, 3.0, 6.0, 3.0};
    auto   m         = Matrix<double>(2, 2, m_data);

    double soll_L_data[4] = {1.0, 0.0, 1.5, 1.0};
    auto   soll_L         = Matrix<double>(2, 2, soll_L_data);

    double soll_U_data[4] = {4.0, 3.0, 0.0, -1.5};
    auto   soll_U         = Matrix<double>(2, 2, soll_U_data);

    Decomposition::LUResult res           = Decomposition::luDecomposition(m,false);
    Matrix<double>          lowerTriangle = res.L;
    Matrix<double>          upperTriangle = res.U;

    ASSERT_TRUE(soll_L.compare(lowerTriangle));
    ASSERT_TRUE(soll_U.compare(upperTriangle));
}

TEST(Decomposition, LU3x3)
{
    // https://en.wikipedia.org/wiki/LU_decomposition

    double m_data[9] = {4.0, 1.0, 7.0, 3.0, 5.0, 10.0, 1.0, -4.0, 2.0};
    auto   m         = Matrix<double>(3, 3, m_data);

    Decomposition::LUResult res = Decomposition::luDecomposition(m,false);

    ASSERT_TRUE(m.compare(res.L * res.U));
}

TEST(Decomposition, LUIdent)
{
    // https://en.wikipedia.org/wiki/LU_decomposition
    auto in = Matrix<double>(6, 6);
    in.setToIdentity();

    Decomposition::LUResult res = Decomposition::luDecomposition(in,true);

    // Both should be identity
    ASSERT_TRUE(in.compare(res.L));
    ASSERT_TRUE(in.compare(res.U));
}


// example from https://math.stackexchange.com/questions/485513/what-are-pivot-numbers-in-lu-decomposition-please-explain-me-in-an-example
TEST(Decomposition, LUPivoting)
{
    double matData[] = {2,1,1,0,  4,3,3,1,  8,7,9,5,  6,7,9,8};
    auto mat = Matrix<double>(4,4,matData);

    double l_data[] = {1,0,0,0,  0.75,1,0,0,  0.5,-2.0/7.0,1,0,  0.25,-3.0/7.0,1.0/3.0,1 };
    auto l = Matrix<double>(4,4, l_data);

    double u_data[] = {8,7,9,5,  0,1.75,2.25,4.25,  0,0,-6.0/7.0,-2.0/7.0,  0,0,0,2.0/3.0};
    auto u = Matrix<double>(4,4,u_data);

    double p_data[] = {0,0,1,0,  0,0,0,1,  0,1,0,0,  1,0,0,0};
    auto p = Matrix<double>(4,4,p_data);

    Decomposition::LUResult res = Decomposition::luDecomposition(mat,true);

    ASSERT_TRUE( (p*mat).compare(l*u) );
    ASSERT_TRUE( (res.P*mat).compare(res.L*res.U) );
    ASSERT_TRUE( l.compare(res.L) );
    ASSERT_TRUE( u.compare(res.U) );
    ASSERT_TRUE( p.compare(res.P) );
    ASSERT_EQ(res.NbrRowSwaps, 3 );
}


TEST(Decomposition, LU2x2Pivoting)
{
    double matData[] = {0, -2,   1, 1};
    auto mat = Matrix<double>(2,2, matData);

    double l_data[] = {1,0, 0,1};
    auto l = Matrix<double>(2,2, l_data);
    double u_data[] = {1,1, 0,-2};
    auto u = Matrix<double>(2,2,u_data);
    double p_data[] = {0,1, 1,0};
    auto p = Matrix<double>(2,2,p_data);

    Decomposition::LUResult res = Decomposition::luDecomposition(mat,true);

    ASSERT_TRUE( (p*mat).compare(l*u) );
    ASSERT_TRUE( l.compare(res.L) );
    ASSERT_TRUE( u.compare(res.U) );
    ASSERT_TRUE( p.compare(res.P) );
}

TEST(Decomposition, EigenvalueSymmetricDiagonalQR)
{
    auto m = Matrix<double>::identity(3);
    for (size_t s = 0; s < 3; s++)
        m(s, s) = static_cast<double>(s + 1);

    std::vector<Decomposition::EigenPair> eig = Decomposition::eigen(m, Decomposition::QRAlgorithm);

    ASSERT_EQ(3, eig.size());

    for( size_t i = 0; i < eig.size(); i++ )
    {
        auto cEP = eig.at(i);

        ASSERT_TRUE( cEP.Valid );
        ASSERT_TRUE( (m * cEP.V).compare( cEP.L * cEP.V, true, 0.00001 ) );
    }
}

TEST(Decomposition, EigenvalueSymmetricDiagonalPower)
{
    auto m = Matrix<double>::identity(3);
    for (size_t s = 0; s < 3; s++)
        m(s, s) = static_cast<double>(s + 1);

    std::vector<Decomposition::EigenPair> eig = Decomposition::eigen(m, Decomposition::PowerIterationAndHotellingsDeflation);

    ASSERT_EQ(3, eig.size());

    for( size_t i = 0; i < eig.size(); i++ )
    {
        auto cEP = eig.at(i);

        ASSERT_TRUE( cEP.Valid );
        ASSERT_TRUE( (m * cEP.V).compare( cEP.L * cEP.V, true, 0.001 ) );
    }
}


TEST(Decomposition, EigenvalueSymmetric)
{
    double data[] = { 6,10,11,  10,17,21,  11,21,42};
    auto m = Matrix<double>(3,3,data);

    // qr is called
    std::vector<Decomposition::EigenPair> eig = Decomposition::eigen(m);

    ASSERT_EQ(3, eig.size());

    // just check if properties are fullfilled.
    for( size_t i = 0; i < eig.size(); i++ )
    {
        auto cEP = eig.at(i);

        ASSERT_TRUE( cEP.Valid );
        ASSERT_TRUE( (m * cEP.V).compare( cEP.L * cEP.V, true, 0.00001 ) );
    }
}

TEST(Decomposition, EigenvalueSymmetricBatchTesting)
{
    for( int k = 0; k < 100; k++ )
    {
        auto m = Matrix<double>::random(5, 5, -10.0, 10.0);
        m = m * m.transpose();

        std::vector<Decomposition::EigenPair> eig = Decomposition::eigen(m);
        for( size_t i = 0; i < eig.size(); i++ )
        {
            auto cEP = eig.at(i);

            if( cEP.Valid )
            {
                ASSERT_TRUE((m * cEP.V).compare(cEP.L * cEP.V, true, 0.00001));
            }
        }
    }
}

// Example from https://en.wikipedia.org/wiki/Rayleigh_quotient_iteration
TEST(Decomposition, EigenvalueNonSymmetricLargest)
{
    double mat_data[] = {1,2,3,  1,2,1,  3,2,1};
    auto m = Matrix<double>(3,3,mat_data);

    double sollEigenVal = 3 + std::sqrt(5.0);
    double sollEigenVecData[] = {1, (1+std::sqrt(5.0))/2.0 - 1  , 1};
    auto sollEigenVec = Matrix<double>(3,1,sollEigenVecData).normalizeColumns();

    Decomposition::EigenPair sigEigenPair = Decomposition::powerIteration(m,40,std::numeric_limits<double>::epsilon());

    ASSERT_TRUE( sigEigenPair.Valid );
    ASSERT_FLOAT_EQ(sigEigenPair.L,sollEigenVal);
    ASSERT_TRUE(sollEigenVec.compare(sigEigenPair.V));


    // This calls also the power iteration
    std::vector<Decomposition::EigenPair> eigenPair = Decomposition::eigen(m);

    ASSERT_TRUE( eigenPair.at(0).Valid );
    ASSERT_FLOAT_EQ(eigenPair.at(0).L,sollEigenVal);
    ASSERT_TRUE(sollEigenVec.compare(eigenPair.at(0).V));
}

/*
// Example from https://en.wikipedia.org/wiki/Rayleigh_quotient_iteration
TEST(Decomposition, EigenvalueAllNonSymmetric)
{
    double mat_data[] = {1,2,3,  1,2,1,  3,2,1};
    auto m = Matrix<double>(3,3,mat_data);

    std::vector<Decomposition::EigenPair> eig = Decomposition::eigen(m);

    for (const Decomposition::EigenPair ep : eig)
    {
        if(ep.Valid)
            std::cout << "Valid" << std::endl;
        else
            std::cout << "Invalid" << std::endl;

        std::cout << "Eigenvalue: " << ep.L << std::endl;
        std::cout << "Eigenvector: " << ep.V.transpose() ;

        if( (m*ep.V).compare(ep.L*ep.V) )
        {
            std::cout << "True" << std::endl;
        }
        else
        {
            auto eucDist = ((m*ep.V) - (ep.L*ep.V));
            std::cout << "False: err = " << eucDist.transpose() << " length: " <<std::sqrt( (eucDist.transpose() * eucDist)(0,0)) << std::endl;
        }

        std::cout << "------" << std::endl << std::endl;
    }
}
*/

// example from document /documents/qr_decomposition.pdf
TEST(Decomposition, QRDecomposition)
{
    double matData[] = {0.8147, 0.0975, 0.1576,
                        0.9058, 0.2785, 0.9706,
                        0.1270, 0.5469, 0.9572,
                        0.9134, 0.9575, 0.4854,
                        0.6324, 0.9649, 0.8003 };

    auto mat = Matrix<double>(5, 3, matData);

    /*
    double qData[] = {0.4927,-0.4806, -0.1780,-0.6015,-0.3644,
                      0.5478,-0.3583,0.5777,0.3760, 0.3104,
                      0.0768,0.4754,0.6343,-0.1497,-0.5859,
                      0.5523,0.3391,-0.4808,0.5071,-0.3026,
                      0.3824,0.5473,-0.0311,-0.4661, 0.5796};
    auto qSoll = Matrix<double>(5,5,qData); */

    double rData[] = {1.6536,1.1405,1.2569,
                      0, 0.9661, 0.6341,
                      0, 0, 0.8816,
                      0, 0, 0,
                      0, 0, 0};
    auto rSoll = Matrix<double>(5,3,rData);

    Decomposition::QRResult res = Decomposition::qr(mat);

    ASSERT_TRUE( res.Q.isOrthogonal(0.01)); // directly comparing Q does not make sense, since signs can change
    ASSERT_TRUE( rSoll.compare(res.R, true, 0.01));
    ASSERT_TRUE( mat.compare( res.Q * res.R, true, 0.01));
}

TEST(Decomposition, QRDecompositionSpecialMatrix)
{
    // Rotation Matrix
    Matrix4x4 rotMat;
    rotMat.rotZ(0.2); rotMat.rotX(0.4);

    Decomposition::QRResult res = Decomposition::qr(rotMat);

    ASSERT_TRUE( rotMat.compare( res.Q * res.R, true, 0.0001 ));

    // check if Q orthogonal
    for( size_t i = 0; i < 4; i++ )
    {
        ASSERT_FLOAT_EQ( res.Q.column(i).norm(), 1.0 );
        ASSERT_FLOAT_EQ( res.Q.row(i).norm(), 1.0 );
    }
}

TEST(Decomposition, QRBatchTesting)
{
    for( int k = 0; k < 500; k++ )
    {
        auto m = Matrix<double>::random(5, 5, -100.0, 100.0);

        Decomposition::QRResult res = Decomposition::qr(m,true);
        auto Q = res.Q;
        auto R = res.R;

        ASSERT_TRUE( Q.isOrthogonal(0.0001) );
        ASSERT_TRUE( m.compare(Q*R, true, 0.0001) );
    }
}

TEST(Decomposition, QRSignChanger)
{
    double matData[] = {0.8147, 0.0975, 0.1576,
                        0.9058, 0.2785, 0.9706,
                        0.1270, 0.5469, 0.9572,
                        0.9134, 0.9575, 0.4854,
                        0.6324, 0.9649, 0.8003 };

    auto mat = Matrix<double>(5, 3, matData);

    Decomposition::QRResult res = Decomposition::qr(mat);

    for( size_t k = 0; k < res.R.rows(); k++ )
    {
        Decomposition::QRResult nextRes = Decomposition::qrSignModifier(res.Q, res.R, k);
        ASSERT_TRUE(mat.compare(nextRes.Q * nextRes.R, true, 0.001));
    }
}

TEST(Decomposition, RQBatchTesting)
{
    for( int k = 0; k < 500; k++ )
    {
        auto m = Matrix<double>::random(6, 6, -10.0, 10.0);

        Decomposition::QRResult res = Decomposition::rq(m);

        auto R = res.R;
        auto Q = res.Q;

        ASSERT_TRUE( Q.isOrthogonal(0.001) );
        ASSERT_TRUE( m.compare(R*Q, true, 0.001) );
    }
}


TEST(Decomposition, HouseHolderBatchTesting)
{
    size_t rows = 5;

    for( int k = 0; k < 100; k++ )
    {
        auto x = Matrix<double>::random(rows, 1, -100.0, 100.0);

        Decomposition::HouseholderResult res = Decomposition::householder(x);
        auto p = Decomposition::householderMatrix(res.V, res.B);
        ASSERT_TRUE( p.isOrthogonal(0.0001) );
        ASSERT_TRUE( p.isSymmetric() );

        // rx should be a multiple of e1 with length x -> rx = [ +/- norm(x), 0, 0, ... 0 ]
        auto rx = p * x;

        auto rxSoll = Matrix<double>(rows, 1);
        rxSoll.fill(0.0);
        rxSoll(0,0) = x.norm();

        ASSERT_TRUE(rxSoll.compare(rx, true, 0.00001) || rxSoll.compare(rx*(-1.0),true, 0.00001) );
    }
}

bool testDiagonalizationResult( const Matrix<double>& a, Decomposition::DiagonalizationResult res)
{
    bool ok = true;

    ok = ok && res.V.isOrthogonal(0.0001) ;
    ok = ok && res.U.isOrthogonal(0.0001) ;
    ok = ok && res.D.compare(  res.U.transpose() * a * res.V, true, 0.0001 );
    ok = ok && a.compare(  res.U * res.D * res.V.transpose(), true, 0.0001 );

    return ok;
}

TEST(Decomposition, BidiagonalizationSymmetric)
{
/* Octave example
A =
   6   8   8   7
   7   8   6   5
   4   2   3   7
   9   1   4   8

U =
   0.4447496  -0.6879265  -0.0015212   0.5735439
   0.5188745  -0.1063169   0.6637464  -0.5281161
   0.2964997  -0.2746100  -0.7223026  -0.5612097
   0.6671244   0.6633575  -0.1942097   0.2778209

B =
   13.49074  -18.90026    0.00000    0.00000
    0.00000    6.65700   -4.13058    0.00000
    0.00000    0.00000    5.03030    0.19847
    0.00000    0.00000    0.00000    1.02931

V =

   1.00000   0.00000   0.00000   0.00000
   0.00000  -0.47455   0.74583  -0.46748
   0.00000  -0.54122   0.17160   0.82318
   0.00000  -0.69418  -0.64365  -0.32223
*/

    // Compared to the example, signs might change
    double aData[] = {6,8,8,7,  7,8,6,5,   4,2,3,7,   9,1,4,8};
    auto a = Matrix<double>(4,4,aData);
    Decomposition::DiagonalizationResult res = Decomposition::bidiagonalization(a);

    ASSERT_TRUE(testDiagonalizationResult(a, res));
}

TEST(Decomposition, BidiagonalizationNonSymmetric)
{
/* Octave example
A =
           1           4           1           6
          10           0           6           4
           7          10           2           7
           8           4           5           3
           7          10           0           1


U =
   -0.061663      0.6189     0.44282     0.16385     0.62468
    -0.61663    -0.50111     0.26481     0.53573     0.10737
    -0.43164     0.58508   -0.019502     0.19214    -0.65884
     -0.4933   -0.078335     0.32425     -0.8033   0.0097606
    -0.43164      0.1319    -0.79263    -0.06282     0.40507

B =
      16.217     -15.114           0           0
           0      8.8768     -6.5868           0
           0           0      4.7876      5.4597
           0           0           0      2.3784
           0           0           0           0

V =
          -1          -0          -0          -0
          -0     0.71806    -0.44909    -0.53171
          -0     0.46918     0.87662    -0.10678
          -0     0.51406    -0.17279     0.84017
*/

    // Compared to the example, signs might change
    double aData[] = {1,4,1,6,    10,0,6,4,    7,10,2,7,    8,4,5,3,     7,10,0,1 };
    auto a = Matrix<double>(4,4,aData);
    Decomposition::DiagonalizationResult res = Decomposition::bidiagonalization(a);

    ASSERT_TRUE(testDiagonalizationResult(a, res));
}


TEST(Decomposition, BidiagonalizationBatchTest)
{
    size_t rows = 6;

    for( int k = 0; k < 200; k++ )
    {
        auto a = Matrix<double>::random(rows, rows, -100.0, 100.0);

        Decomposition::DiagonalizationResult res = Decomposition::bidiagonalization(a);
        ASSERT_TRUE(testDiagonalizationResult(a, res));
    }
}

// http://mysite.science.uottawa.ca/phofstra/MAT2342/SVDproblems.pdf
TEST(Decomposition, SVD)
{
    double matData[] = {0.0, 1.0, 1.0,
                        std::sqrt(2.0), 2.0, 0,
                        0.0, 1.0, 1.0};

    auto mat = Matrix<double>(3, 3, matData);

    Matrix<double> s_soll = Matrix<double>(3,3);
    s_soll.fill(0.0);
    s_soll(0,0) = 2.0 * std::sqrt(2.0);
    s_soll(1,1) = std::sqrt(2.0);

    /*
    double u_soll_data[] = {1.0/std::sqrt(6.0),   -1.0/std::sqrt(3.0),   1/std::sqrt(2.0),
                       2.0/std::sqrt(6.0),   1.0/std::sqrt(3.0),       0.0,
                       1.0/std::sqrt(6.0),   -1.0/std::sqrt(3.0),   -1.0/std::sqrt(2.0)};
    auto u_soll = Matrix<double>(3,3,u_soll_data);

    double v_soll_data[] = {1.0/std::sqrt(6.0),   1.0/std::sqrt(3.0),    1/std::sqrt(2.0),
                            3.0/std::sqrt(12.0),   0.0,           -0.5,
                            1.0/std::sqrt(12.0),   -2.0/std::sqrt(6.0),   0.5};
    auto v_soll = Matrix<double>(3,3,v_soll_data); */


    Decomposition::SVDResult res = Decomposition::svd(mat);

    // U and V cannot be compared directly, because it can change in signs.
    ASSERT_TRUE( res.U.isOrthogonal(0.001) );
    ASSERT_TRUE( res.V.isOrthogonal(0.001) );

    ASSERT_TRUE( s_soll.compare(res.S, true, 0.001) );

    ASSERT_TRUE( mat.compare(res.U * res.S * res.V.transpose(), true, 0.001 ) );
}

TEST(Decomposition, SVDBatch)
{
    size_t rows = 5;

    for( int k = 0; k < 500; k++ )
    {
        auto a = Matrix<double>::random(rows, rows, -5.0, 5.0);

        Decomposition::SVDResult res = Decomposition::svd(a);

        ASSERT_TRUE( res.U.isOrthogonal(0.05) );
        ASSERT_TRUE( res.V.isOrthogonal(0.05) );
        ASSERT_TRUE( a.compare(res.U * res.S * res.V.transpose(), true, 0.05 ) );
    }
}

