#include "MatrixTests.hpp"

#include <cassert>
#include <cmath>

#include "RectangularMatrix.hpp"
#include "SquareMatrix.hpp"
#include "DiagonalMatrix.hpp"
#include "TriangularMatrix.hpp"

static bool AreEqual(double left, double right, double epsilon = 1e-9)
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

void TestDiagonalMatrix()
{
    int data[] = {1, 2, 3};
    DiagonalMatrix<int> matrix(data, 3);

    assert(matrix.GetSize() == 3);
    assert(matrix.GetRowCount() == 3);
    assert(matrix.GetColumnCount() == 3);
    assert(matrix.Get(0, 0) == 1);
    assert(matrix.Get(1, 1) == 2);
    assert(matrix.Get(2, 2) == 3);
    assert(matrix.Get(0, 2) == 0);

    matrix.Set(1, 1, 10);
    matrix.Set(0, 2, 0);
    assert(matrix.Get(1, 1) == 10);

    Matrix<int>* clone = matrix.Clone();
    DiagonalMatrix<int>* diagonalClone = dynamic_cast<DiagonalMatrix<int>*>(clone);
    assert(diagonalClone != nullptr);
    assert(diagonalClone->Get(1, 1) == 10);
    delete clone;

    Matrix<int>* scaled = matrix.MultiplyByScalar(2);
    DiagonalMatrix<int>* diagonalScaled = dynamic_cast<DiagonalMatrix<int>*>(scaled);
    assert(diagonalScaled != nullptr);
    assert(diagonalScaled->Get(0, 0) == 2);
    assert(diagonalScaled->Get(1, 1) == 20);
    assert(diagonalScaled->Get(0, 1) == 0);
    delete scaled;

    Matrix<int>* sum = matrix.Add(matrix);
    DiagonalMatrix<int>* diagonalSum = dynamic_cast<DiagonalMatrix<int>*>(sum);
    assert(diagonalSum != nullptr);
    assert(diagonalSum->Get(0, 0) == 2);
    assert(diagonalSum->Get(1, 1) == 20);
    delete sum;

    RectangularMatrix<int> dense(3, 3, 1);
    Matrix<int>* denseSum = matrix.Add(dense);
    SquareMatrix<int>* squareSum = dynamic_cast<SquareMatrix<int>*>(denseSum);
    assert(squareSum != nullptr);
    assert(squareSum->Get(0, 0) == 2);
    assert(squareSum->Get(0, 1) == 1);
    assert(squareSum->Get(1, 1) == 11);
    delete denseSum;

    DiagonalMatrix<double> normMatrix(3, 0.0);
    normMatrix.Set(0, 0, 3.0);
    normMatrix.Set(1, 1, 4.0);
    assert(AreEqual(normMatrix.Norm(), 5.0));

    bool thrown = false;
    try
    {
        matrix.Set(0, 1, 7);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        matrix.SwapRows(0, 1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

void TestTriangularMatrix()
{
    int upperData[] = {
        1, 2, 3,
        0, 4, 5,
        0, 0, 6
    };

    TriangularMatrix<int> upper(upperData, 3, TriangularType::Upper);

    assert(upper.GetSize() == 3);
    assert(upper.GetType() == TriangularType::Upper);
    assert(upper.Get(0, 0) == 1);
    assert(upper.Get(0, 2) == 3);
    assert(upper.Get(1, 2) == 5);
    assert(upper.Get(2, 0) == 0);

    upper.Set(0, 2, 30);
    upper.Set(2, 0, 0);
    assert(upper.Get(0, 2) == 30);

    Matrix<int>* clone = upper.Clone();
    TriangularMatrix<int>* triangularClone = dynamic_cast<TriangularMatrix<int>*>(clone);
    assert(triangularClone != nullptr);
    assert(triangularClone->GetType() == TriangularType::Upper);
    assert(triangularClone->Get(0, 2) == 30);
    delete clone;

    Matrix<int>* scaled = upper.MultiplyByScalar(2);
    TriangularMatrix<int>* triangularScaled = dynamic_cast<TriangularMatrix<int>*>(scaled);
    assert(triangularScaled != nullptr);
    assert(triangularScaled->GetType() == TriangularType::Upper);
    assert(triangularScaled->Get(0, 2) == 60);
    assert(triangularScaled->Get(2, 0) == 0);
    delete scaled;

    Matrix<int>* sum = upper.Add(upper);
    TriangularMatrix<int>* triangularSum = dynamic_cast<TriangularMatrix<int>*>(sum);
    assert(triangularSum != nullptr);
    assert(triangularSum->GetType() == TriangularType::Upper);
    assert(triangularSum->Get(0, 2) == 60);
    assert(triangularSum->Get(2, 0) == 0);
    delete sum;

    int lowerData[] = {
        1, 0, 0,
        2, 3, 0,
        4, 5, 6
    };

    TriangularMatrix<int> lower(lowerData, 3, TriangularType::Lower);
    assert(lower.GetType() == TriangularType::Lower);
    assert(lower.Get(2, 0) == 4);
    assert(lower.Get(0, 2) == 0);

    lower.AddRowMultiple(0, 2, 2);
    assert(lower.Get(2, 0) == 6);
    assert(lower.Get(2, 1) == 5);
    assert(lower.Get(2, 2) == 6);

    upper.AddColumnMultiple(0, 2, 1);
    assert(upper.Get(0, 2) == 31);
    assert(upper.Get(1, 2) == 5);

    bool thrown = false;
    try
    {
        upper.Set(2, 0, 7);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        upper.AddRowMultiple(0, 2, 1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        TriangularMatrix<int> bad(upperData, 3, TriangularType::Lower);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
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

    thrown = false;
    try
    {
        DiagonalMatrix<int> badDiagonal(-1, 0);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        DiagonalMatrix<int> badDiagonal(nullptr, 2);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        TriangularMatrix<int> badTriangular(-1, TriangularType::Upper, 0);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        TriangularMatrix<int> badTriangular(nullptr, 2, TriangularType::Upper);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        DiagonalMatrix<int> left(2, 1);
        TriangularMatrix<int> right(3, TriangularType::Upper, 1);
        left.Add(right);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}
