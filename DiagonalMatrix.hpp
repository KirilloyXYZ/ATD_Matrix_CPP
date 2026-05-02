#ifndef DIAGONALMATRIX_HPP
#define DIAGONALMATRIX_HPP

#include <cmath>
#include <stdexcept>

#include "Exceptions.h"
#include "Matrix.hpp"
#include "MutableArraySequence.hpp"
#include "ScalarTraits.hpp"
#include "SquareMatrix.hpp"

template<typename T>
class DiagonalMatrix : public Matrix<T>
{
private:
    MutableArraySequence<T> diagonal;
    int size;
    T zeroValue;

    void ValidateSize(int size) const;
    void CheckIndex(int row, int column) const;
    void CheckSingleIndex(int index) const;
    void CheckSameSize(const Matrix<T>& other) const;
    bool IsZero(const T& value) const;

public:
    DiagonalMatrix();
    explicit DiagonalMatrix(int size, const T& defaultValue = T());
    DiagonalMatrix(const T* items, int size);
    DiagonalMatrix(const DiagonalMatrix<T>& other);
    DiagonalMatrix<T>& operator=(const DiagonalMatrix<T>& other);
    ~DiagonalMatrix() override;

    int GetSize() const;

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
DiagonalMatrix<T>::DiagonalMatrix() : diagonal(), size(0), zeroValue(T()) { }

template<typename T>
DiagonalMatrix<T>::DiagonalMatrix(int size, const T& defaultValue)
    : diagonal(), size(0), zeroValue(T())
{
    ValidateSize(size);
    this->size = size;

    for (int i = 0; i < size; ++i)
    {
        diagonal.Append(defaultValue);
    }
}

template<typename T>
DiagonalMatrix<T>::DiagonalMatrix(const T* items, int size)
    : diagonal(), size(0), zeroValue(T())
{
    ValidateSize(size);

    if (items == nullptr && size > 0)
    {
        throw std::invalid_argument("DiagonalMatrix constructor: null items with positive size");
    }

    this->size = size;

    for (int i = 0; i < size; ++i)
    {
        diagonal.Append(items[i]);
    }
}

template<typename T>
DiagonalMatrix<T>::DiagonalMatrix(const DiagonalMatrix<T>& other)
    : diagonal(other.diagonal), size(other.size), zeroValue(T()) { }

template<typename T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator=(const DiagonalMatrix<T>& other)
{
    if (this != &other)
    {
        diagonal = other.diagonal;
        size = other.size;
        zeroValue = T();
    }

    return *this;
}

template<typename T>
DiagonalMatrix<T>::~DiagonalMatrix()
    = default;

template<typename T>
void DiagonalMatrix<T>::ValidateSize(int size) const
{
    if (size < 0)
    {
        throw std::invalid_argument("DiagonalMatrix: size cannot be negative");
    }
}

template<typename T>
void DiagonalMatrix<T>::CheckSingleIndex(int index) const
{
    if (index < 0 || index >= size)
    {
        throw IndexOutOfRange("DiagonalMatrix: index out of range");
    }
}

template<typename T>
void DiagonalMatrix<T>::CheckIndex(int row, int column) const
{
    CheckSingleIndex(row);
    CheckSingleIndex(column);
}

template<typename T>
void DiagonalMatrix<T>::CheckSameSize(const Matrix<T>& other) const
{
    if (size != other.GetRowCount() || size != other.GetColumnCount())
    {
        throw std::invalid_argument("DiagonalMatrix::Add: matrices must have equal dimensions");
    }
}

template<typename T>
bool DiagonalMatrix<T>::IsZero(const T& value) const
{
    return value == T();
}

template<typename T>
int DiagonalMatrix<T>::GetSize() const
{
    return size;
}

template<typename T>
int DiagonalMatrix<T>::GetRowCount() const
{
    return size;
}

template<typename T>
int DiagonalMatrix<T>::GetColumnCount() const
{
    return size;
}

template<typename T>
const T& DiagonalMatrix<T>::Get(int row, int column) const
{
    CheckIndex(row, column);

    if (row == column)
    {
        return diagonal.Get(row);
    }

    return zeroValue;
}

template<typename T>
void DiagonalMatrix<T>::Set(int row, int column, const T& value)
{
    CheckIndex(row, column);

    if (row == column)
    {
        diagonal.Set(row, value);
        return;
    }

    if (!IsZero(value))
    {
        throw std::invalid_argument("DiagonalMatrix::Set: non-zero value outside diagonal");
    }
}

template<typename T>
Matrix<T>* DiagonalMatrix<T>::Clone() const
{
    return new DiagonalMatrix<T>(*this);
}

template<typename T>
Matrix<T>* DiagonalMatrix<T>::Add(const Matrix<T>& other) const
{
    CheckSameSize(other);

    const DiagonalMatrix<T>* diagonalOther = dynamic_cast<const DiagonalMatrix<T>*>(&other);

    if (diagonalOther != nullptr)
    {
        DiagonalMatrix<T>* result = new DiagonalMatrix<T>(size);

        for (int i = 0; i < size; ++i)
        {
            result->Set(i, i, diagonal.Get(i) + diagonalOther->diagonal.Get(i));
        }

        return result;
    }

    SquareMatrix<T>* result = new SquareMatrix<T>(size);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            result->Set(i, j, Get(i, j) + other.Get(i, j));
        }
    }

    return result;
}

template<typename T>
Matrix<T>* DiagonalMatrix<T>::MultiplyByScalar(const T& scalar) const
{
    DiagonalMatrix<T>* result = new DiagonalMatrix<T>(size);

    for (int i = 0; i < size; ++i)
    {
        result->Set(i, i, diagonal.Get(i) * scalar);
    }

    return result;
}

template<typename T>
double DiagonalMatrix<T>::Norm() const
{
    double sum = 0.0;

    for (int i = 0; i < size; ++i)
    {
        sum += ScalarTraits<T>::MagnitudeSquared(diagonal.Get(i));
    }

    return std::sqrt(sum);
}

template<typename T>
void DiagonalMatrix<T>::SwapRows(int firstRow, int secondRow)
{
    CheckSingleIndex(firstRow);
    CheckSingleIndex(secondRow);

    if (firstRow != secondRow)
    {
        throw std::invalid_argument("DiagonalMatrix::SwapRows: operation breaks diagonal structure");
    }
}

template<typename T>
void DiagonalMatrix<T>::SwapColumns(int firstColumn, int secondColumn)
{
    CheckSingleIndex(firstColumn);
    CheckSingleIndex(secondColumn);

    if (firstColumn != secondColumn)
    {
        throw std::invalid_argument("DiagonalMatrix::SwapColumns: operation breaks diagonal structure");
    }
}

template<typename T>
void DiagonalMatrix<T>::MultiplyRow(int row, const T& scalar)
{
    CheckSingleIndex(row);
    diagonal.Set(row, diagonal.Get(row) * scalar);
}

template<typename T>
void DiagonalMatrix<T>::MultiplyColumn(int column, const T& scalar)
{
    CheckSingleIndex(column);
    diagonal.Set(column, diagonal.Get(column) * scalar);
}

template<typename T>
void DiagonalMatrix<T>::AddRowMultiple(int sourceRow, int targetRow, const T& scalar)
{
    CheckSingleIndex(sourceRow);
    CheckSingleIndex(targetRow);

    if (sourceRow == targetRow)
    {
        diagonal.Set(targetRow, diagonal.Get(targetRow) + diagonal.Get(sourceRow) * scalar);
        return;
    }

    if (!IsZero(diagonal.Get(sourceRow) * scalar))
    {
        throw std::invalid_argument("DiagonalMatrix::AddRowMultiple: operation breaks diagonal structure");
    }
}

template<typename T>
void DiagonalMatrix<T>::AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar)
{
    CheckSingleIndex(sourceColumn);
    CheckSingleIndex(targetColumn);

    if (sourceColumn == targetColumn)
    {
        diagonal.Set(targetColumn, diagonal.Get(targetColumn) + diagonal.Get(sourceColumn) * scalar);
        return;
    }

    if (!IsZero(diagonal.Get(sourceColumn) * scalar))
    {
        throw std::invalid_argument("DiagonalMatrix::AddColumnMultiple: operation breaks diagonal structure");
    }
}

#endif
