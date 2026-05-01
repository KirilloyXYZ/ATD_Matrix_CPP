#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Matrix.hpp"
#include "RectangularMatrix.hpp"
#include "SquareMatrix.hpp"
#include "LinearSystemSolvers.hpp"

void ClearInput()
{
    std::cin.clear();
    while (std::cin.get() != '\n' && std::cin)
    {
    }
}

int ReadInt(const std::string& prompt)
{
    int value;

    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            return value;
        }

        std::cout << "Invalid integer. Try again.\n";
        ClearInput();
    }
}

double ReadDouble(const std::string& prompt)
{
    double value;

    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            return value;
        }

        std::cout << "Invalid number. Try again.\n";
        ClearInput();
    }
}

Matrix<int>*& SelectMatrixSlot(Matrix<int>*& matrixA, Matrix<int>*& matrixB)
{
    int choice = ReadInt("Select matrix (1 = A, 2 = B): ");

    if (choice == 1)
    {
        return matrixA;
    }

    if (choice == 2)
    {
        return matrixB;
    }

    throw std::invalid_argument("Unknown matrix slot");
}

Matrix<int>* GetExistingMatrix(Matrix<int>* matrixA, Matrix<int>* matrixB)
{
    Matrix<int>*& selected = SelectMatrixSlot(matrixA, matrixB);

    if (selected == nullptr)
    {
        throw std::runtime_error("Selected matrix is not created");
    }

    return selected;
}

void ReplaceMatrix(Matrix<int>*& target, Matrix<int>* result)
{
    if (result != target)
    {
        delete target;
        target = result;
    }
}

template<typename T>
void FillGeneratedValues(Matrix<T>& matrix, T startValue, T step)
{
    T currentValue = startValue;

    for (int i = 0; i < matrix.GetRowCount(); ++i)
    {
        for (int j = 0; j < matrix.GetColumnCount(); ++j)
        {
            matrix.Set(i, j, currentValue);
            currentValue += step;
        }
    }
}

void PrintDoubleVector(const std::string& name, const Sequence<double>& vector)
{
    std::cout << name << ": [";

    for (int i = 0; i < vector.GetLength(); ++i)
    {
        if (i > 0)
        {
            std::cout << ", ";
        }

        std::cout << vector.Get(i);
    }

    std::cout << "]\n";
}

void PrintDoubleMatrix(const std::string& name, const Matrix<double>& matrix)
{
    std::cout << name << " (" << matrix.GetRowCount() << " x " << matrix.GetColumnCount() << "):\n";

    for (int i = 0; i < matrix.GetRowCount(); ++i)
    {
        std::cout << "  [";

        for (int j = 0; j < matrix.GetColumnCount(); ++j)
        {
            if (j > 0)
            {
                std::cout << ", ";
            }

            std::cout << matrix.Get(i, j);
        }

        std::cout << "]\n";
    }
}

Matrix<int>* CreateManualMatrix()
{
    int type = ReadInt("Matrix type (1 = rectangular, 2 = square): ");

    if (type == 1)
    {
        int rows = ReadInt("Rows: ");
        int columns = ReadInt("Columns: ");

        RectangularMatrix<int>* matrix = new RectangularMatrix<int>(rows, columns, 0);

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < columns; ++j)
            {
                matrix->Set(i, j, ReadInt("value[" + std::to_string(i) + "][" + std::to_string(j) + "]: "));
            }
        }

        return matrix;
    }

    if (type == 2)
    {
        int size = ReadInt("Size: ");

        SquareMatrix<int>* matrix = new SquareMatrix<int>(size, 0);

        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                matrix->Set(i, j, ReadInt("value[" + std::to_string(i) + "][" + std::to_string(j) + "]: "));
            }
        }

        return matrix;
    }

    throw std::invalid_argument("Unknown matrix type");
}

Matrix<int>* CreateGeneratedMatrix()
{
    int type = ReadInt("Matrix type (1 = rectangular, 2 = square): ");
    int startValue = ReadInt("Start value: ");
    int step = ReadInt("Step: ");

    if (type == 1)
    {
        int rows = ReadInt("Rows: ");
        int columns = ReadInt("Columns: ");

        RectangularMatrix<int>* matrix = new RectangularMatrix<int>(rows, columns, 0);
        FillGeneratedValues(*matrix, startValue, step);
        return matrix;
    }

    if (type == 2)
    {
        int size = ReadInt("Size: ");

        SquareMatrix<int>* matrix = new SquareMatrix<int>(size, 0);
        FillGeneratedValues(*matrix, startValue, step);
        return matrix;
    }

    throw std::invalid_argument("Unknown matrix type");
}

void PrintMatrix(const std::string& name, const Matrix<int>* matrix)
{
    std::cout << name << ": ";

    if (matrix == nullptr)
    {
        std::cout << "<not created>\n";
        return;
    }

    const SquareMatrix<int>* squareMatrix = dynamic_cast<const SquareMatrix<int>*>(matrix); // а объект, на который указывает matrix, реально является SquareMatrix<int>? или это прямоугольная ?


    if (squareMatrix != nullptr)
    {
        std::cout << "SquareMatrix<int> ";
    }
    else
    {
        std::cout << "RectangularMatrix<int> ";
    }

    std::cout << "(" << matrix->GetRowCount() << " x " << matrix->GetColumnCount() << ")\n";

    for (int i = 0; i < matrix->GetRowCount(); ++i)
    {
        std::cout << "  [";

        for (int j = 0; j < matrix->GetColumnCount(); ++j)
        {
            if (j > 0)
            {
                std::cout << ", ";
            }

            std::cout << matrix->Get(i, j);
        }

        std::cout << "]\n";
    }
}

void RunPerformanceDemo()
{
    int size = ReadInt("Square matrix size for benchmark: ");
    int repeats = ReadInt("Repeats: ");

    if (size < 0)
    {
        throw std::invalid_argument("Size cannot be negative");
    }

    if (repeats <= 0)
    {
        throw std::invalid_argument("Repeats must be positive");
    }

    RectangularMatrix<int> first(size, size, 0);
    RectangularMatrix<int> second(size, size, 0);

    FillGeneratedValues(first, 1, 1);
    FillGeneratedValues(second, 2, 1);

    auto addStart = std::chrono::steady_clock::now();
    for (int i = 0; i < repeats; ++i)
    {
        Matrix<int>* result = first.Add(second);
        delete result;
    }
    auto addFinish = std::chrono::steady_clock::now();

    auto scalarStart = std::chrono::steady_clock::now();
    for (int i = 0; i < repeats; ++i)
    {
        Matrix<int>* result = first.MultiplyByScalar(2);
        delete result;
    }
    auto scalarFinish = std::chrono::steady_clock::now();

    double lastNorm = 0.0;
    auto normStart = std::chrono::steady_clock::now();
    for (int i = 0; i < repeats; ++i)
    {
        lastNorm = first.Norm();
    }
    auto normFinish = std::chrono::steady_clock::now();

    double addMs = std::chrono::duration<double, std::milli>(addFinish - addStart).count();
    double scalarMs = std::chrono::duration<double, std::milli>(scalarFinish - scalarStart).count();
    double normMs = std::chrono::duration<double, std::milli>(normFinish - normStart).count();

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Benchmark for " << size << " x " << size << " matrix\n";
    std::cout << "  Add average time: " << (addMs / repeats) << " ms\n";
    std::cout << "  Scalar multiply average time: " << (scalarMs / repeats) << " ms\n";
    std::cout << "  Norm average time: " << (normMs / repeats) << " ms\n";
    std::cout << "  Last norm value: " << lastNorm << "\n";
}

void PrintSolverReport(const std::string& name, const Matrix<double>& coefficients, const Sequence<double>& rightSide, const Sequence<double>& exactSolution, const Sequence<double>& solution)

{
    std::cout << name
              << " residual = " << ComputeResidualNorm(coefficients, solution, rightSide)
              << ", relative error = " << ComputeRelativeError(solution, exactSolution)
              << "\n";
}

void PrintSolverResidual(const std::string& name, const Matrix<double>& coefficients, const Sequence<double>& rightSide, const Sequence<double>& solution)
{
    std::cout << name
              << " residual = " << ComputeResidualNorm(coefficients, solution, rightSide)
              << "\n";
}

void PrintManualSolverResult(const std::string& name, MutableArraySequence<double> (*solver)(const Matrix<double>&, const Sequence<double>&), const Matrix<double>& coefficients, const Sequence<double>& rightSide)
{
    try
    {
        MutableArraySequence<double> solution = solver(coefficients, rightSide);
        PrintDoubleVector(name, solution);
        PrintSolverResidual(name, coefficients, rightSide, solution);
    }
    catch (const std::exception& ex)
    {
        std::cout << name << " failed: " << ex.what() << "\n";
    }
}

void RunLinearSystemDemo()
{
    int size = ReadInt("System size: ");

    if (size <= 0)
    {
        throw std::invalid_argument("System size must be positive");
    }

    RectangularMatrix<double> coefficients(size, size, 0.0);
    FillGeneratedValues(coefficients, 1.0, 1.0);

    for (int i = 0; i < size; ++i)
    {
        coefficients.Set(i, i, coefficients.Get(i, i) + size * size);
    }

    MutableArraySequence<double> exactSolution = CreateOnesVector(size);
    MutableArraySequence<double> rightSide = MultiplyMatrixByVector(coefficients, exactSolution);

    MutableArraySequence<double> noPivotSolution = SolveGaussNoPivot(coefficients, rightSide);
    MutableArraySequence<double> pivotSolution = SolveGaussPartialPivot(coefficients, rightSide);
    MutableArraySequence<double> luSolution = SolveLU(coefficients, rightSide);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Generated diagonally dominant system\n";
    std::cout << "Solving equation: A * x = b\n";

    if (size <= 10)
    {
        PrintDoubleMatrix("Matrix A", coefficients);
        PrintDoubleVector("Right side b", rightSide);
        PrintDoubleVector("Exact solution x", exactSolution);
    }
    else
    {
        std::cout << "Matrix A is " << size << " x " << size << " and is not printed.\n";
    }

    std::cout << "\nSolutions\n";
    PrintDoubleVector("Gauss without pivot", noPivotSolution);
    PrintDoubleVector("Gauss with partial pivot", pivotSolution);
    PrintDoubleVector("LU decomposition", luSolution);

    std::cout << "\nQuality check\n";
    PrintSolverReport("Gauss no pivot", coefficients, rightSide, exactSolution, noPivotSolution);
    PrintSolverReport("Gauss pivot", coefficients, rightSide, exactSolution, pivotSolution);
    PrintSolverReport("LU", coefficients, rightSide, exactSolution, luSolution);
    std::cout << std::defaultfloat << std::setprecision(6);
}

void RunManualLinearSystemDemo()
{
    int size = ReadInt("System size: ");

    if (size <= 0)
    {
        throw std::invalid_argument("System size must be positive");
    }

    RectangularMatrix<double> coefficients(size, size, 0.0);
    MutableArraySequence<double> rightSide;

    std::cout << "Enter coefficients of matrix A\n";
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            coefficients.Set(i, j, ReadDouble("A[" + std::to_string(i) + "][" + std::to_string(j) + "]: "));
        }
    }

    std::cout << "Enter right side vector b\n";
    for (int i = 0; i < size; ++i)
    {
        rightSide.Append(ReadDouble("b[" + std::to_string(i) + "]: "));
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Manual linear system\n";
    std::cout << "Solving equation: A * x = b\n";
    PrintDoubleMatrix("Matrix A", coefficients);
    PrintDoubleVector("Right side b", rightSide);

    std::cout << "\nSolutions\n";
    PrintManualSolverResult("Gauss without pivot", SolveGaussNoPivot, coefficients, rightSide);
    PrintManualSolverResult("Gauss with partial pivot", SolveGaussPartialPivot, coefficients, rightSide);
    PrintManualSolverResult("LU decomposition", SolveLU, coefficients, rightSide);
    std::cout << std::defaultfloat << std::setprecision(6);
}

void PrintMenu()
{
    std::cout << "\n===== LAB 3 MENU =====\n";
    std::cout << "1. Create matrix A manually\n";
    std::cout << "2. Create matrix B manually\n";
    std::cout << "3. Create matrix automatically\n";
    std::cout << "4. Print matrices\n";
    std::cout << "5. Get matrix element\n";
    std::cout << "6. Set matrix element\n";
    std::cout << "7. Add A + B and save result\n";
    std::cout << "8. Multiply matrix by scalar\n";
    std::cout << "9. Compute matrix norm\n";
    std::cout << "10. Swap rows\n";
    std::cout << "11. Swap columns\n";
    std::cout << "12. Multiply row by scalar\n";
    std::cout << "13. Add one row to another with coefficient\n";
    std::cout << "14. Multiply column by scalar\n";
    std::cout << "15. Add one column to another with coefficient\n";
    std::cout << "16. Run performance demo\n";
    std::cout << "17. Run linear system demo (Gauss and LU)\n";
    std::cout << "18. Solve manual linear system\n";
    std::cout << "0. Exit\n";
}

int main()
{
    Matrix<int>* matrixA = nullptr;
    Matrix<int>* matrixB = nullptr;

    bool running = true;

    while (running)
    {
        try
        {
            PrintMenu();
            int command = ReadInt("Choose: ");

            switch (command)
            {
                case 1:
                {
                    ReplaceMatrix(matrixA, CreateManualMatrix());
                    std::cout << "Matrix A created.\n";
                    break;
                }

                case 2:
                {
                    ReplaceMatrix(matrixB, CreateManualMatrix());
                    std::cout << "Matrix B created.\n";
                    break;
                }

                case 3:
                {
                    Matrix<int>*& target = SelectMatrixSlot(matrixA, matrixB);
                    ReplaceMatrix(target, CreateGeneratedMatrix());
                    std::cout << "Matrix created.\n";
                    break;
                }

                case 4:
                {
                    PrintMatrix("Matrix A", matrixA);
                    PrintMatrix("Matrix B", matrixB);
                    break;
                }

                case 5:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int row = ReadInt("Row: ");
                    int column = ReadInt("Column: ");
                    std::cout << "Value = " << matrix->Get(row, column) << "\n";
                    break;
                }

                case 6:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int row = ReadInt("Row: ");
                    int column = ReadInt("Column: ");
                    int value = ReadInt("New value: ");
                    matrix->Set(row, column, value);
                    PrintMatrix("Updated matrix", matrix);
                    break;
                }

                case 7:
                {
                    if (matrixA == nullptr || matrixB == nullptr)
                    {
                        throw std::runtime_error("Create matrices A and B first");
                    }

                    Matrix<int>* result = matrixA->Add(*matrixB);
                    PrintMatrix("A + B", result);

                    Matrix<int>*& target = SelectMatrixSlot(matrixA, matrixB);
                    ReplaceMatrix(target, result);
                    std::cout << "Result saved.\n";
                    break;
                }

                case 8:
                {
                    Matrix<int>*& target = SelectMatrixSlot(matrixA, matrixB);

                    if (target == nullptr)
                    {
                        throw std::runtime_error("Selected matrix is not created");
                    }

                    int scalar = ReadInt("Scalar: ");
                    ReplaceMatrix(target, target->MultiplyByScalar(scalar));
                    PrintMatrix("Updated matrix", target);
                    break;
                }

                case 9:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    std::cout << std::fixed << std::setprecision(3);
                    std::cout << "Norm = " << matrix->Norm() << "\n";
                    break;
                }

                case 10:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int firstRow = ReadInt("First row: ");
                    int secondRow = ReadInt("Second row: ");
                    matrix->SwapRows(firstRow, secondRow);
                    PrintMatrix("Updated matrix", matrix);
                    break;
                }

                case 11:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int firstColumn = ReadInt("First column: ");
                    int secondColumn = ReadInt("Second column: ");
                    matrix->SwapColumns(firstColumn, secondColumn);
                    PrintMatrix("Updated matrix", matrix);
                    break;
                }

                case 12:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int row = ReadInt("Row: ");
                    int scalar = ReadInt("Scalar: ");
                    matrix->MultiplyRow(row, scalar);
                    PrintMatrix("Updated matrix", matrix);
                    break;
                }

                case 13:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int sourceRow = ReadInt("Source row: ");
                    int targetRow = ReadInt("Target row: ");
                    int scalar = ReadInt("Coefficient: ");
                    matrix->AddRowMultiple(sourceRow, targetRow, scalar);
                    PrintMatrix("Updated matrix", matrix);
                    break;
                }

                case 14:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int column = ReadInt("Column: ");
                    int scalar = ReadInt("Scalar: ");
                    matrix->MultiplyColumn(column, scalar);
                    PrintMatrix("Updated matrix", matrix);
                    break;
                }

                case 15:
                {
                    Matrix<int>* matrix = GetExistingMatrix(matrixA, matrixB);
                    int sourceColumn = ReadInt("Source column: ");
                    int targetColumn = ReadInt("Target column: ");
                    int scalar = ReadInt("Coefficient: ");
                    matrix->AddColumnMultiple(sourceColumn, targetColumn, scalar);
                    PrintMatrix("Updated matrix", matrix);
                    break;
                }

                case 16:
                {
                    RunPerformanceDemo();
                    break;
                }

                case 17:
                {
                    RunLinearSystemDemo();
                    break;
                }

                case 18:
                {
                    RunManualLinearSystemDemo();
                    break;
                }

                case 0:
                {
                    running = false;
                    break;
                }

                default:
                {
                    std::cout << "Unknown command.\n";
                    break;
                }
            }
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << "\n";
        }
    }

    delete matrixA;
    delete matrixB;

    return 0;
}
