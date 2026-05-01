#ifndef LINEARSYSTEMTESTS_HPP
#define LINEARSYSTEMTESTS_HPP

#include <cassert>
#include <cmath>

#include "LinearSystemSolvers.hpp"

bool LinearAreClose(double left, double right, double epsilon = 1e-9)
{
    return std::fabs(left - right) <= epsilon;
}

void AssertVectorClose(const Sequence<double>& actual, const Sequence<double>& expected, double epsilon = 1e-9)
{
    assert(actual.GetLength() == expected.GetLength());

    for (int i = 0; i < actual.GetLength(); ++i)
    {
        assert(LinearAreClose(actual.Get(i), expected.Get(i), epsilon));
    }
}

void TestGaussSolvers()
{
    double data[] = {
        2.0, 1.0,
        5.0, 7.0
    };

    RectangularMatrix<double> matrix(data, 2, 2);
    MutableArraySequence<double> expected;
    expected.Append(1.0);
    expected.Append(1.0);

    MutableArraySequence<double> rightSide = MultiplyMatrixByVector(matrix, expected);

    MutableArraySequence<double> noPivot = SolveGaussNoPivot(matrix, rightSide);
    MutableArraySequence<double> partialPivot = SolveGaussPartialPivot(matrix, rightSide);

    AssertVectorClose(noPivot, expected);
    AssertVectorClose(partialPivot, expected);
    assert(LinearAreClose(ComputeResidualNorm(matrix, partialPivot, rightSide), 0.0));
    assert(LinearAreClose(ComputeRelativeError(partialPivot, expected), 0.0));
}

void TestGaussPartialPivot()
{
    double data[] = {
        0.0, 2.0,
        1.0, 1.0
    };

    RectangularMatrix<double> matrix(data, 2, 2);
    MutableArraySequence<double> rightSide;
    rightSide.Append(2.0);
    rightSide.Append(2.0);

    MutableArraySequence<double> expected;
    expected.Append(1.0);
    expected.Append(1.0);

    MutableArraySequence<double> solution = SolveGaussPartialPivot(matrix, rightSide);
    AssertVectorClose(solution, expected);

    bool thrown = false;

    try
    {
        SolveGaussNoPivot(matrix, rightSide);
    }
    catch (...)
    {
        thrown = true;
    }

    assert(thrown);
}

void TestLUDecompositionAndSolve()
{
    double data[] = {
        4.0, 3.0,
        6.0, 3.0
    };

    RectangularMatrix<double> matrix(data, 2, 2);
    MutableArraySequence<double> expected;
    expected.Append(1.0);
    expected.Append(2.0);

    MutableArraySequence<double> rightSide = MultiplyMatrixByVector(matrix, expected);

    LUDecomposition decomposition = DecomposeLU(matrix);

    assert(LinearAreClose(decomposition.lower.Get(0, 0), 1.0));
    assert(LinearAreClose(decomposition.lower.Get(1, 0), 1.5));
    assert(LinearAreClose(decomposition.upper.Get(0, 0), 4.0));
    assert(LinearAreClose(decomposition.upper.Get(0, 1), 3.0));
    assert(LinearAreClose(decomposition.upper.Get(1, 1), -1.5));

    MutableArraySequence<double> solution = SolveLU(matrix, rightSide);
    AssertVectorClose(solution, expected);
}

void TestLinearSystemErrors()
{
    double data[] = {
        1.0, 2.0,
        2.0, 4.0
    };

    RectangularMatrix<double> singular(data, 2, 2);
    MutableArraySequence<double> rightSide;
    rightSide.Append(1.0);
    rightSide.Append(2.0);

    bool thrown = false;

    try
    {
        DecomposeLU(singular);
    }
    catch (...)
    {
        thrown = true;
    }

    assert(thrown);

    RectangularMatrix<double> matrix(2, 2, 1.0);
    MutableArraySequence<double> badRightSide;
    badRightSide.Append(1.0);

    thrown = false;

    try
    {
        SolveGaussPartialPivot(matrix, badRightSide);
    }
    catch (...)
    {
        thrown = true;
    }

    assert(thrown);
}

#endif
