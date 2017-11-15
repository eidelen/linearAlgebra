#include <gtest/gtest.h>
#include "matrix.hpp"

TEST(MatMul, Scalar)
{
    size_t s = 5;
    auto mat = Matrix<int>(s,s);
    mat.setValue(3);

    auto res1 = mat * 2; // res should be 2*3 = 6

    auto soll1 = Matrix<int>(s,s);
    soll1.setValue(6);

    ASSERT_TRUE(res1.compare(soll1));

    auto res2 = 2 * mat; // res should be 2*3 = 6
    ASSERT_TRUE(res2.compare(soll1));
}


TEST(MatMul, MatrixMatrixSquare)
{
    auto m1 = Matrix<int>(2,2); m1.setValue(2);
    auto m2 = Matrix<int>(2,2); m2.setValue(3);

    auto res1 = m1 * m2; // 2*3*2 = 12

    auto soll1 = Matrix<int>(2,2); soll1.setValue(12);
    ASSERT_TRUE(res1.compare(soll1));

    m2(1,1) = 1;
    auto res2 = m1 * m2; // 2*3*2 = 12 and res2(:,1) = 2*3 + 1*2 = 8;

    auto soll2 = Matrix<int>(2,2); soll2.setValue(12); soll2(0,1) = 8; soll2(1,1) = 8;

    ASSERT_TRUE(res2.compare(soll2));
}

TEST(MatMul, VectorVector)
{
    auto v1 = Matrix<int>(1,3); v1.setValue(2);
    auto v2 = Matrix<int>(3,1); v2.setValue(5); v2(2,0) = 1;

    // 2*5 * 2  + 2*1 = 22

    auto res = v1 * v2;

    ASSERT_EQ(res.cols() , 1);
    ASSERT_EQ(res.rows() , 1);
    ASSERT_EQ(res(0,0) , 22);
}




