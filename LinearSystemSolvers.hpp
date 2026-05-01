#ifndef LINEARSYSTEMSOLVERS_HPP
#define LINEARSYSTEMSOLVERS_HPP

#include "Matrix.hpp"
#include "MutableArraySequence.hpp"
#include "RectangularMatrix.hpp"
#include "Sequence.hpp"
#include "SquareMatrix.hpp"

struct LUDecomposition
{
    SquareMatrix<double> lower;
    SquareMatrix<double> upper;
};

MutableArraySequence<double> CreateOnesVector(int size);
MutableArraySequence<double> MultiplyMatrixByVector(const Matrix<double>& matrix, const Sequence<double>& vector);
double VectorNorm(const Sequence<double>& vector);
double ComputeResidualNorm(const Matrix<double>& matrix, const Sequence<double>& solution, const Sequence<double>& rightSide);

double ComputeRelativeError(const Sequence<double>& approximate, const Sequence<double>& exact);

MutableArraySequence<double> SolveGaussNoPivot(const Matrix<double>& matrix, const Sequence<double>& rightSide);
MutableArraySequence<double> SolveGaussPartialPivot(const Matrix<double>& matrix, const Sequence<double>& rightSide);

LUDecomposition DecomposeLU(const Matrix<double>& matrix);
MutableArraySequence<double> ForwardSubstitution(const Matrix<double>& lower, const Sequence<double>& rightSide);
MutableArraySequence<double> BackwardSubstitution(const Matrix<double>& upper, const Sequence<double>& rightSide);
MutableArraySequence<double> SolveLU(const Matrix<double>& matrix, const Sequence<double>& rightSide);

#endif
