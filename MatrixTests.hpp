#ifndef MATRIXTESTS_HPP
#define MATRIXTESTS_HPP

#include <cassert>
#include <cmath>

#include "RectangularMatrix.hpp"
#include "SquareMatrix.hpp"

bool AreEqual(double left, double right, double epsilon = 1e-9)
{
    return std::fabs(left - right) <= epsilon;
}

void TestRectangularMatrixCreation()
{
    int data[] = {1, 2, 3, 4, 5, 6};
    RectangularMatrix<int> matrix(data, 2, 3);

    assert(matrix.GetRowCount() == 2);
    assert(matrix.GetColumnCount() == 3);
    assert(matrix.Get(0, 0) == 1);
    assert(matrix.Get(0, 2) == 3);
    assert(matrix.Get(1, 0) == 4);
    assert(matrix.Get(1, 2) == 6);

    matrix.Set(1, 1, 99);
    assert(matrix.Get(1, 1) == 99);
}

void TestRectangularMatrixCopy()
{
    RectangularMatrix<int> original(2, 2, 0);

    original.Set(0, 0, 1);
    original.Set(0, 1, 2);
    original.Set(1, 0, 3);
    original.Set(1, 1, 4);

    RectangularMatrix<int> copy(original);
    original.Set(0, 0, 100);

    assert(copy.Get(0, 0) == 1);
    assert(copy.Get(1, 1) == 4);

    RectangularMatrix<int> assigned;
    assigned = original;
    original.Set(1, 1, 500);

    assert(assigned.Get(0, 0) == 100);
    assert(assigned.Get(1, 1) == 4);
}

void TestRectangularMatrixOperations()
{
    int leftData[] = {1, 2, 3, 4};
    int rightData[] = {5, 6, 7, 8};

    RectangularMatrix<int> left(leftData, 2, 2);
    RectangularMatrix<int> right(rightData, 2, 2);

    Matrix<int>* sum = left.Add(right);
    assert(sum->Get(0, 0) == 6);
    assert(sum->Get(0, 1) == 8);
    assert(sum->Get(1, 0) == 10);
    assert(sum->Get(1, 1) == 12);
    delete sum;

    Matrix<int>* scaled = left.MultiplyByScalar(3);
    assert(scaled->Get(0, 0) == 3);
    assert(scaled->Get(1, 1) == 12);
    delete scaled;

    RectangularMatrix<double> normMatrix(1, 2, 0.0);
    normMatrix.Set(0, 0, 3.0);
    normMatrix.Set(0, 1, 4.0);
    assert(AreEqual(normMatrix.Norm(), 5.0));
}

void TestRectangularMatrixRowAndColumnOperations()
{
    int data[] = {
        1, 2, 3,
        4, 5, 6
    };

    RectangularMatrix<int> matrix(data, 2, 3);

    matrix.SwapRows(0, 1);
    assert(matrix.Get(0, 0) == 4);
    assert(matrix.Get(1, 2) == 3);

    matrix.SwapColumns(0, 2);
    assert(matrix.Get(0, 0) == 6);
    assert(matrix.Get(0, 2) == 4);

    matrix.MultiplyRow(0, 2);
    assert(matrix.Get(0, 0) == 12);
    assert(matrix.Get(0, 1) == 10);
    assert(matrix.Get(0, 2) == 8);

    matrix.AddRowMultiple(0, 1, 1);
    assert(matrix.Get(1, 0) == 15);
    assert(matrix.Get(1, 1) == 12);
    assert(matrix.Get(1, 2) == 9);

    matrix.MultiplyColumn(1, -1);
    assert(matrix.Get(0, 1) == -10);
    assert(matrix.Get(1, 1) == -12);

    matrix.AddColumnMultiple(0, 2, -1);
    assert(matrix.Get(0, 2) == -4);
    assert(matrix.Get(1, 2) == -6);
}

void TestSquareMatrix()
{
    int data[] = {
        1, 2,
        3, 4
    };

    SquareMatrix<int> matrix(data, 2);
    assert(matrix.GetSize() == 2);
    assert(matrix.Get(1, 0) == 3);

    Matrix<int>* clone = matrix.Clone();
    SquareMatrix<int>* squareClone = dynamic_cast<SquareMatrix<int>*>(clone);
    assert(squareClone != nullptr);
    assert(squareClone->Get(0, 1) == 2);
    delete clone;

    Matrix<int>* sum = matrix.Add(matrix);
    SquareMatrix<int>* squareSum = dynamic_cast<SquareMatrix<int>*>(sum);
    assert(squareSum != nullptr);
    assert(squareSum->Get(0, 0) == 2);
    assert(squareSum->Get(1, 1) == 8);
    delete sum;
}

void TestMatrixErrors()
{
    bool thrown = false;

    try
    {
        RectangularMatrix<int> bad(-1, 2, 0);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        RectangularMatrix<int> matrix(2, 2, 0);
        matrix.Get(10, 0);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        RectangularMatrix<int> left(2, 2, 1);
        RectangularMatrix<int> right(2, 3, 1);
        left.Add(right);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        RectangularMatrix<int> matrix(nullptr, 2, 2);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

#endif
