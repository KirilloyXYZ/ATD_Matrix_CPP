#ifndef RECTANGULARMATRIX_HPP
#define RECTANGULARMATRIX_HPP

#include <cmath>
#include <stdexcept>

#include "Matrix.hpp"
#include "MutableArraySequence.hpp"
#include "ScalarTraits.hpp"

template<typename T>
class RectangularMatrix : public Matrix<T>
{
private:
    MutableArraySequence<MutableArraySequence<T>> rows;
    int rowCount;
    int columnCount;

    void ValidateDimensions(int rows, int columns) const;
    void CheckRowIndex(int row) const;
    void CheckColumnIndex(int column) const;
    void CheckIndex(int row, int column) const;
    void CheckSameSize(const Matrix<T>& other) const;

public:
    RectangularMatrix();
    RectangularMatrix(int rows, int columns, const T& defaultValue = T());
    RectangularMatrix(const T* items, int rows, int columns);
    RectangularMatrix(const RectangularMatrix<T>& other);
    RectangularMatrix<T>& operator=(const RectangularMatrix<T>& other);
    ~RectangularMatrix() override;

    int GetRowCount() const override;
    int GetColumnCount() const override;

    const T& Get(int row, int column) const override;
    void Set(int row, int column, const T& value) override;

    Matrix<T>* Clone() const override;
    Matrix<T>* Add(const Matrix<T>& other) const override;
    Matrix<T>* MultiplyByScalar(const T& scalar) const override;
    double Norm() const override;

    void SwapRows(int firstRow, int secondRow) override;
    void SwapColumns(int firstColumn, int secondColumn) override;
    void MultiplyRow(int row, const T& scalar) override;
    void MultiplyColumn(int column, const T& scalar) override;
    void AddRowMultiple(int sourceRow, int targetRow, const T& scalar) override;
    void AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar) override;
};

template<typename T>
RectangularMatrix<T>::RectangularMatrix() : rows(), rowCount(0), columnCount(0) { }

template<typename T>
RectangularMatrix<T>::RectangularMatrix(int rows, int columns, const T& defaultValue)
    : rows(), rowCount(0), columnCount(0)
{
    ValidateDimensions(rows, columns);

    this->rowCount = rows;
    this->columnCount = columns;

    for (int i = 0; i < rows; ++i)
    {


        this->rows.Append(MutableArraySequence<T>());
        MutableArraySequence<T>& row = this->rows.GetMutable(i);

        for (int j = 0; j < columns; ++j)
        {
            row.Append(defaultValue);
        }
    }
}

template<typename T>
RectangularMatrix<T>::RectangularMatrix(const T* items, int rows, int columns)
    : rows(), rowCount(0), columnCount(0)
{
    ValidateDimensions(rows, columns);

    if (items == nullptr && rows * columns > 0)
    {
        throw std::invalid_argument("RectangularMatrix constructor: null items with positive size");
    }

    this->rowCount = rows;
    this->columnCount = columns;

    for (int i = 0; i < rows; ++i)
    {


        this->rows.Append(MutableArraySequence<T>());
        MutableArraySequence<T>& row = this->rows.GetMutable(i);

        for (int j = 0; j < columns; ++j)
        {
            row.Append(items[i * columns + j]);
        }
    }
}

template<typename T>
RectangularMatrix<T>::RectangularMatrix(const RectangularMatrix<T>& other)
    : rows(other.rows), rowCount(other.rowCount), columnCount(other.columnCount) { }

template<typename T>
RectangularMatrix<T>& RectangularMatrix<T>::operator=(const RectangularMatrix<T>& other)
{
    if (this != &other)
    {
        rows = other.rows;
        rowCount = other.rowCount;
        columnCount = other.columnCount;
    }

    return *this;
}

template<typename T>
RectangularMatrix<T>::~RectangularMatrix()
    = default;

template<typename T>
void RectangularMatrix<T>::ValidateDimensions(int rows, int columns) const
{
    if (rows < 0 || columns < 0)
    {
        throw std::invalid_argument("RectangularMatrix: dimensions cannot be negative");
    }
}

template<typename T>
void RectangularMatrix<T>::CheckRowIndex(int row) const
{
    if (row < 0 || row >= rowCount)
    {
        throw IndexOutOfRange("RectangularMatrix: row index out of range");
    }
}

template<typename T>
void RectangularMatrix<T>::CheckColumnIndex(int column) const
{
    if (column < 0 || column >= columnCount)
    {
        throw IndexOutOfRange("RectangularMatrix: column index out of range");
    }
}

template<typename T>
void RectangularMatrix<T>::CheckIndex(int row, int column) const
{
    CheckRowIndex(row);
    CheckColumnIndex(column);
}

template<typename T>
void RectangularMatrix<T>::CheckSameSize(const Matrix<T>& other) const
{
    if (rowCount != other.GetRowCount() || columnCount != other.GetColumnCount())
    {
        throw std::invalid_argument("RectangularMatrix::Add: matrices must have equal dimensions");
    }
}

template<typename T>
int RectangularMatrix<T>::GetRowCount() const
{
    return rowCount;
}

template<typename T>
int RectangularMatrix<T>::GetColumnCount() const
{
    return columnCount;
}

template<typename T>
const T& RectangularMatrix<T>::Get(int row, int column) const
{
    CheckIndex(row, column);
    return rows.Get(row).Get(column);
}

template<typename T>
void RectangularMatrix<T>::Set(int row, int column, const T& value)
{
    CheckIndex(row, column);
    rows.GetMutable(row).Set(column, value);
}

template<typename T>
Matrix<T>* RectangularMatrix<T>::Clone() const
{
    return new RectangularMatrix<T>(*this);
}

template<typename T>
Matrix<T>* RectangularMatrix<T>::Add(const Matrix<T>& other) const
{
    CheckSameSize(other);

    RectangularMatrix<T>* result = new RectangularMatrix<T>(rowCount, columnCount);

    for (int i = 0; i < rowCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
        {
            result->Set(i, j, Get(i, j) + other.Get(i, j));
        }
    }

    return result;
}

template<typename T>
Matrix<T>* RectangularMatrix<T>::MultiplyByScalar(const T& scalar) const
{
    RectangularMatrix<T>* result = new RectangularMatrix<T>(rowCount, columnCount);

    for (int i = 0; i < rowCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
        {
            result->Set(i, j, Get(i, j) * scalar);
        }
    }

    return result;
}

template<typename T>
double RectangularMatrix<T>::Norm() const
{
    double sum = 0.0;

    for (int i = 0; i < rowCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
        {
            sum += ScalarTraits<T>::MagnitudeSquared(Get(i, j));
        }
    }

    return std::sqrt(sum);
}

template<typename T>
void RectangularMatrix<T>::SwapRows(int firstRow, int secondRow)
{
    CheckRowIndex(firstRow);
    CheckRowIndex(secondRow);

    if (firstRow == secondRow)
    {
        return;
    }

    MutableArraySequence<T> first = rows.Get(firstRow);
    rows.Set(firstRow, rows.Get(secondRow));
    rows.Set(secondRow, first);
}

template<typename T>
void RectangularMatrix<T>::SwapColumns(int firstColumn, int secondColumn)
{
    CheckColumnIndex(firstColumn);
    CheckColumnIndex(secondColumn);

    if (firstColumn == secondColumn)
    {
        return;
    }

    for (int i = 0; i < rowCount; ++i)
    {
        T first = Get(i, firstColumn);
        T second = Get(i, secondColumn);

        Set(i, firstColumn, second);
        Set(i, secondColumn, first);
    }
}

template<typename T>
void RectangularMatrix<T>::MultiplyRow(int row, const T& scalar)
{
    CheckRowIndex(row);

    for (int j = 0; j < columnCount; ++j)
    {
        Set(row, j, Get(row, j) * scalar);
    }
}

template<typename T>
void RectangularMatrix<T>::MultiplyColumn(int column, const T& scalar)
{
    CheckColumnIndex(column);

    for (int i = 0; i < rowCount; ++i)
    {
        Set(i, column, Get(i, column) * scalar);
    }
}

template<typename T>
void RectangularMatrix<T>::AddRowMultiple(int sourceRow, int targetRow, const T& scalar)
{
    CheckRowIndex(sourceRow);
    CheckRowIndex(targetRow);

    for (int j = 0; j < columnCount; ++j)
    {
        Set(targetRow, j, Get(targetRow, j) + Get(sourceRow, j) * scalar);
    }
}

template<typename T>
void RectangularMatrix<T>::AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar)
{
    CheckColumnIndex(sourceColumn);
    CheckColumnIndex(targetColumn);

    for (int i = 0; i < rowCount; ++i)
    {
        Set(i, targetColumn, Get(i, targetColumn) + Get(i, sourceColumn) * scalar);
    }
}

#endif
