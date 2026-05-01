#include "LinearSystemSolvers.hpp"

#include <cmath>
#include <stdexcept>

static const double PIVOT_EPSILON = 1e-12;

static void ValidateVectorSize(const Sequence<double>& vector, int expectedSize, const char* message)
{
    if (vector.GetLength() != expectedSize)
    {
        throw std::invalid_argument(message);
    }
}

static void ValidateSquareMatrix(const Matrix<double>& matrix, const char* message)
{
    if (matrix.GetRowCount() != matrix.GetColumnCount())
    {
        throw std::invalid_argument(message);
    }
}

static void ValidateLinearSystem(const Matrix<double>& matrix, const Sequence<double>& rightSide)
{
    ValidateSquareMatrix(matrix, "Linear system: matrix must be square");
    ValidateVectorSize(rightSide, matrix.GetRowCount(), "Linear system: right side size mismatch");
}

static RectangularMatrix<double> CopyMatrix(const Matrix<double>& matrix)
{
    RectangularMatrix<double> copy(matrix.GetRowCount(), matrix.GetColumnCount(), 0.0);

    for (int i = 0; i < matrix.GetRowCount(); ++i)
    {
        for (int j = 0; j < matrix.GetColumnCount(); ++j)
        {
            copy.Set(i, j, matrix.Get(i, j));
        }
    }

    return copy;
}

static MutableArraySequence<double> CopyVector(const Sequence<double>& vector)
{
    MutableArraySequence<double> copy;

    for (int i = 0; i < vector.GetLength(); ++i)
    {
        copy.Append(vector.Get(i));
    }

    return copy;
}

static MutableArraySequence<double> CreateZeroVector(int size)
{
    if (size < 0)
    {
        throw std::invalid_argument("Vector size cannot be negative");
    }

    MutableArraySequence<double> vector;

    for (int i = 0; i < size; ++i)
    {
        vector.Append(0.0);
    }

    return vector;
}

static void CheckPivot(double pivot, const char* message)
{
    if (std::abs(pivot) < PIVOT_EPSILON)
    {
        throw std::invalid_argument(message);
    }
}

static void SwapVectorValues(MutableArraySequence<double>& vector, int firstIndex, int secondIndex)
{
    double first = vector.Get(firstIndex);
    vector.Set(firstIndex, vector.Get(secondIndex));
    vector.Set(secondIndex, first);
}

MutableArraySequence<double> CreateOnesVector(int size)
{
    if (size < 0)
    {
        throw std::invalid_argument("Vector size cannot be negative");
    }

    MutableArraySequence<double> vector;

    for (int i = 0; i < size; ++i)
    {
        vector.Append(1.0);
    }

    return vector;
}

MutableArraySequence<double> MultiplyMatrixByVector(const Matrix<double>& matrix, const Sequence<double>& vector)
{
    ValidateVectorSize(vector, matrix.GetColumnCount(), "MultiplyMatrixByVector: vector size mismatch");

    MutableArraySequence<double> result = CreateZeroVector(matrix.GetRowCount());

    for (int i = 0; i < matrix.GetRowCount(); ++i)
    {
        double sum = 0.0;

        for (int j = 0; j < matrix.GetColumnCount(); ++j)
        {
            sum += matrix.Get(i, j) * vector.Get(j);
        }

        result.Set(i, sum);
    }

    return result;
}

double VectorNorm(const Sequence<double>& vector)
{
    double sum = 0.0;

    for (int i = 0; i < vector.GetLength(); ++i)
    {
        sum += vector.Get(i) * vector.Get(i);
    }

    return std::sqrt(sum);
}

double ComputeResidualNorm(const Matrix<double>& matrix, const Sequence<double>& solution, const Sequence<double>& rightSide)
{
    ValidateVectorSize(solution, matrix.GetColumnCount(), "ComputeResidualNorm: solution size mismatch");
    ValidateVectorSize(rightSide, matrix.GetRowCount(), "ComputeResidualNorm: right side size mismatch");

    MutableArraySequence<double> product = MultiplyMatrixByVector(matrix, solution);
    MutableArraySequence<double> residual;

    for (int i = 0; i < rightSide.GetLength(); ++i)
    {
        residual.Append(product.Get(i) - rightSide.Get(i));
    }

    return VectorNorm(residual);
}

double ComputeRelativeError(const Sequence<double>& approximate, const Sequence<double>& exact)
{
    ValidateVectorSize(approximate, exact.GetLength(), "ComputeRelativeError: vector size mismatch");

    double exactNorm = VectorNorm(exact);

    if (exactNorm < PIVOT_EPSILON)
    {
        throw std::invalid_argument("ComputeRelativeError: exact vector norm is zero");
    }

    MutableArraySequence<double> difference;

    for (int i = 0; i < exact.GetLength(); ++i)
    {
        difference.Append(approximate.Get(i) - exact.Get(i));
    }

    return VectorNorm(difference) / exactNorm;
}

MutableArraySequence<double> SolveGaussNoPivot(const Matrix<double>& matrix, const Sequence<double>& rightSide)
{
    ValidateLinearSystem(matrix, rightSide);

    int n = matrix.GetRowCount();
    RectangularMatrix<double> a = CopyMatrix(matrix);
    MutableArraySequence<double> b = CopyVector(rightSide);

    for (int k = 0; k < n - 1; ++k)
    {
        CheckPivot(a.Get(k, k), "SolveGaussNoPivot: zero pivot");

        for (int i = k + 1; i < n; ++i)
        {
            double factor = a.Get(i, k) / a.Get(k, k);

            for (int j = k; j < n; ++j)
            {
                a.Set(i, j, a.Get(i, j) - factor * a.Get(k, j));
            }

            b.Set(i, b.Get(i) - factor * b.Get(k));
        }
    }

    return BackwardSubstitution(a, b);
}

MutableArraySequence<double> SolveGaussPartialPivot(const Matrix<double>& matrix, const Sequence<double>& rightSide)
{
    ValidateLinearSystem(matrix, rightSide);

    int n = matrix.GetRowCount();
    RectangularMatrix<double> a = CopyMatrix(matrix);
    MutableArraySequence<double> b = CopyVector(rightSide);

    for (int k = 0; k < n - 1; ++k)
    {
        int pivotRow = k;
        double pivotValue = std::abs(a.Get(k, k));

        for (int i = k + 1; i < n; ++i)
        {
            double candidate = std::abs(a.Get(i, k));

            if (candidate > pivotValue)
            {
                pivotValue = candidate;
                pivotRow = i;
            }
        }

        CheckPivot(pivotValue, "SolveGaussPartialPivot: zero pivot");

        if (pivotRow != k)
        {
            a.SwapRows(k, pivotRow);
            SwapVectorValues(b, k, pivotRow);
        }

        for (int i = k + 1; i < n; ++i)
        {
            double factor = a.Get(i, k) / a.Get(k, k);

            for (int j = k; j < n; ++j)
            {
                a.Set(i, j, a.Get(i, j) - factor * a.Get(k, j));
            }

            b.Set(i, b.Get(i) - factor * b.Get(k));
        }
    }

    return BackwardSubstitution(a, b);
}

LUDecomposition DecomposeLU(const Matrix<double>& matrix)
{
    ValidateSquareMatrix(matrix, "DecomposeLU: matrix must be square");

    int n = matrix.GetRowCount();
    LUDecomposition result;
    result.lower = SquareMatrix<double>(n, 0.0);
    result.upper = SquareMatrix<double>(n, 0.0);

    for (int i = 0; i < n; ++i)
    {
        for (int k = i; k < n; ++k)
        {
            double sum = 0.0;

            for (int j = 0; j < i; ++j)
            {
                sum += result.lower.Get(i, j) * result.upper.Get(j, k);
            }

            result.upper.Set(i, k, matrix.Get(i, k) - sum);
        }

        result.lower.Set(i, i, 1.0);
        CheckPivot(result.upper.Get(i, i), "DecomposeLU: zero pivot");

        for (int k = i + 1; k < n; ++k)
        {
            double sum = 0.0;

            for (int j = 0; j < i; ++j)
            {
                sum += result.lower.Get(k, j) * result.upper.Get(j, i);
            }

            result.lower.Set(k, i, (matrix.Get(k, i) - sum) / result.upper.Get(i, i));
        }
    }

    return result;
}

MutableArraySequence<double> ForwardSubstitution(const Matrix<double>& lower, const Sequence<double>& rightSide)
{
    ValidateLinearSystem(lower, rightSide);

    int n = lower.GetRowCount();
    MutableArraySequence<double> result = CreateZeroVector(n);

    for (int i = 0; i < n; ++i)
    {
        CheckPivot(lower.Get(i, i), "ForwardSubstitution: zero pivot");

        double sum = 0.0;

        for (int j = 0; j < i; ++j)
        {
            sum += lower.Get(i, j) * result.Get(j);
        }

        result.Set(i, (rightSide.Get(i) - sum) / lower.Get(i, i));
    }

    return result;
}

MutableArraySequence<double> BackwardSubstitution(const Matrix<double>& upper, const Sequence<double>& rightSide)
{
    ValidateLinearSystem(upper, rightSide);

    int n = upper.GetRowCount();
    MutableArraySequence<double> result = CreateZeroVector(n);

    for (int i = n - 1; i >= 0; --i)
    {
        CheckPivot(upper.Get(i, i), "BackwardSubstitution: zero pivot");

        double sum = 0.0;

        for (int j = i + 1; j < n; ++j)
        {
            sum += upper.Get(i, j) * result.Get(j);
        }

        result.Set(i, (rightSide.Get(i) - sum) / upper.Get(i, i));
    }

    return result;
}

MutableArraySequence<double> SolveLU(const Matrix<double>& matrix, const Sequence<double>& rightSide)
{
    ValidateLinearSystem(matrix, rightSide);

    LUDecomposition decomposition = DecomposeLU(matrix);
    MutableArraySequence<double> intermediate = ForwardSubstitution(decomposition.lower, rightSide);

    return BackwardSubstitution(decomposition.upper, intermediate);
}
