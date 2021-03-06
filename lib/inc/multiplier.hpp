/****************************************************************************
** Copyright (c) 2017 Adrian Schneider
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
**
*****************************************************************************/

#ifndef MY_MULTIPLIER_H
#define MY_MULTIPLIER_H

#include "matrix.hpp"

// tutorial: http://stattrek.com/matrix-algebra/elementary-operations.aspx

class Multiplier
{
public:
    /**
     * Get the matrix L-multiplier, which swaps the rows r0 and r1.
     * @param mat
     * @param r0
     * @param r1
     * @return Mulitplier matrix.
     */
    template <class T>
    static Matrix<T> swapRow(const Matrix<T>& mat, size_t r0, size_t r1);

    /**
     * Get the matrix L-multiplier, which adds the product of factor * r0 and adds to row r1.
     * @param mat
     * @param factor
     * @param r0 Row which is multiplied
     * @param r1 Row to which the product is added
     * @return Multiplier matrix.
     */
    template <class T>
    static Matrix<T> addProductOfRow(const Matrix<T>& mat, T factor, size_t r0, size_t r1);

    /**
     * Get the matrix L-multiplier, which multiplies the row r1 by factor.
     * @tparam T
     * @param mat
     * @param factor
     * @param r
     * @return Multiplier matrix.
     */
    template <class T>
    static Matrix<T> multiplyRow(const Matrix<T>& mat, T factor, size_t r);
};

template <class T>
Matrix<T> Multiplier::swapRow(const Matrix<T>& mat, size_t r0, size_t r1)
{
    if (std::max(r0, r1) >= mat.rows())
    {
        std::cout << "row index exceeds matrix size";
        std::exit(-1);
    }

    Matrix<T> swapOp(mat.rows(), mat.rows());
    swapOp.setToIdentity();

    Matrix<T> zeroRow(1, mat.rows());
    zeroRow.fill(0);

    // r0
    swapOp.setRow(r0, zeroRow);
    swapOp(r0, r1) = 1;

    // r1
    swapOp.setRow(r1, zeroRow);
    swapOp(r1, r0) = 1;

    return swapOp;
}

template <class T>
Matrix<T> Multiplier::addProductOfRow(const Matrix<T>& mat, T factor, size_t r0, size_t r1)
{
    if (std::max(r0, r1) >= mat.rows())
    {
        std::cout << "row index exceeds matrix size";
        std::exit(-1);
    }

    Matrix<T> addProdOp(mat.rows(), mat.rows());
    addProdOp.setToIdentity();
    addProdOp(r1, r0) = factor;

    return addProdOp;
}

template <class T>
Matrix<T> Multiplier::multiplyRow(const Matrix<T>& mat, T factor, size_t r)
{
    if (r >= mat.rows())
    {
        std::cout << "row index exceeds matrix size";
        std::exit(-1);
    }

    Matrix<T> mulOp(mat.rows(), mat.rows());
    mulOp.setToIdentity();
    mulOp(r, r) = factor;

    return mulOp;
}

#endif //MY_MULTIPLIER_H
