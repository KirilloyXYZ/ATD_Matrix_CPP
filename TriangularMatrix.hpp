#ifndef TRIANGULARMATRIX_HPP
#define TRIANGULARMATRIX_HPP

#include <cmath>
#include <stdexcept>

#include "Exceptions.h"
#include "Matrix.hpp"
#include "MutableArraySequence.hpp"
#include "ScalarTraits.hpp"
#include "SquareMatrix.hpp"

enum class TriangularType
{
    Upper,
    Lower
};

template<typename T>
class TriangularMatrix : public Matrix<T>
{
private:
    MutableArraySequence<MutableArraySequence<T>> rows;
    int size;
    TriangularType type;
    T zeroValue;

    void ValidateSize(int size) const;
    void CheckIndex(int row, int column) const;
    void CheckSingleIndex(int index) const;
    void CheckSameSize(const Matrix<T>& other) const;
    bool IsZero(const T& value) const;
    bool IsStoredPosition(int row, int column) const;
    int GetStoredColumnIndex(int row, int column) const;
    int GetStoredColumnStart(int row) const;
    int GetStoredColumnEnd(int row) const;

public:
    TriangularMatrix();
    explicit TriangularMatrix(int size, TriangularType type = TriangularType::Upper, const T& defaultValue = T());
    TriangularMatrix(const T* items, int size, TriangularType type);
    TriangularMatrix(const TriangularMatrix<T>& other);
    TriangularMatrix<T>& operator=(const TriangularMatrix<T>& other);
    ~TriangularMatrix() override;

    int GetSize() const;
    TriangularType GetType() const;

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
TriangularMatrix<T>::TriangularMatrix()
    : rows(), size(0), type(TriangularType::Upper), zeroValue(T()) { }

template<typename T>
TriangularMatrix<T>::TriangularMatrix(int size, TriangularType type, const T& defaultValue)
    : rows(), size(0), type(type), zeroValue(T())
{
    ValidateSize(size);
    this->size = size;

    for (int i = 0; i < size; ++i)
    {
        rows.Append(MutableArraySequence<T>());
        MutableArraySequence<T>& row = rows.GetMutable(i);

        for (int j = GetStoredColumnStart(i); j <= GetStoredColumnEnd(i); ++j)
        {
            row.Append(defaultValue);
        }
    }
}

template<typename T>
TriangularMatrix<T>::TriangularMatrix(const T* items, int size, TriangularType type)
    : TriangularMatrix(size, type, T())
{
    if (items == nullptr && size > 0)
    {
        throw std::invalid_argument("TriangularMatrix constructor: null items with positive size");
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            Set(i, j, items[i * size + j]);
        }
    }
}

template<typename T>
TriangularMatrix<T>::TriangularMatrix(const TriangularMatrix<T>& other)
    : rows(other.rows), size(other.size), type(other.type), zeroValue(T()) { }

template<typename T>
TriangularMatrix<T>& TriangularMatrix<T>::operator=(const TriangularMatrix<T>& other)
{
    if (this != &other)
    {
        rows = other.rows;
        size = other.size;
        type = other.type;
        zeroValue = T();
    }

    return *this;
}

template<typename T>
TriangularMatrix<T>::~TriangularMatrix()
    = default;

template<typename T>
void TriangularMatrix<T>::ValidateSize(int size) const
{
    if (size < 0)
    {
        throw std::invalid_argument("TriangularMatrix: size cannot be negative");
    }
}

template<typename T>
void TriangularMatrix<T>::CheckSingleIndex(int index) const
{
    if (index < 0 || index >= size)
    {
        throw IndexOutOfRange("TriangularMatrix: index out of range");
    }
}

template<typename T>
void TriangularMatrix<T>::CheckIndex(int row, int column) const
{
    CheckSingleIndex(row);
    CheckSingleIndex(column);
}

template<typename T>
void TriangularMatrix<T>::CheckSameSize(const Matrix<T>& other) const
{
    if (size != other.GetRowCount() || size != other.GetColumnCount())
    {
        throw std::invalid_argument("TriangularMatrix::Add: matrices must have equal dimensions");
    }
}

template<typename T>
bool TriangularMatrix<T>::IsZero(const T& value) const
{
    return value == T();
}

template<typename T>
bool TriangularMatrix<T>::IsStoredPosition(int row, int column) const
{
    if (type == TriangularType::Upper)
    {
        return column >= row;
    }

    return column <= row;
}

template<typename T>
int TriangularMatrix<T>::GetStoredColumnIndex(int row, int column) const
{
    if (type == TriangularType::Upper)
    {
        return column - row;
    }

    return column;
}

template<typename T>
int TriangularMatrix<T>::GetStoredColumnStart(int row) const
{
    if (type == TriangularType::Upper)
    {
        return row;
    }

    return 0;
}

template<typename T>
int TriangularMatrix<T>::GetStoredColumnEnd(int row) const
{
    if (type == TriangularType::Upper)
    {
        return size - 1;
    }

    return row;
}

template<typename T>
int TriangularMatrix<T>::GetSize() const
{
    return size;
}

template<typename T>
TriangularType TriangularMatrix<T>::GetType() const
{
    return type;
}

template<typename T>
int TriangularMatrix<T>::GetRowCount() const
{
    return size;
}

template<typename T>
int TriangularMatrix<T>::GetColumnCount() const
{
    return size;
}

template<typename T>
const T& TriangularMatrix<T>::Get(int row, int column) const
{
    CheckIndex(row, column);

    if (IsStoredPosition(row, column))
    {
        return rows.Get(row).Get(GetStoredColumnIndex(row, column));
    }

    return zeroValue;
}

template<typename T>
void TriangularMatrix<T>::Set(int row, int column, const T& value)
{
    CheckIndex(row, column);

    if (IsStoredPosition(row, column))
    {
        rows.GetMutable(row).Set(GetStoredColumnIndex(row, column), value);
        return;
    }

    if (!IsZero(value))
    {
        throw std::invalid_argument("TriangularMatrix::Set: non-zero value outside triangular area");
    }
}

template<typename T>
Matrix<T>* TriangularMatrix<T>::Clone() const
{
    return new TriangularMatrix<T>(*this);
}

template<typename T>
Matrix<T>* TriangularMatrix<T>::Add(const Matrix<T>& other) const
{
    CheckSameSize(other);

    const TriangularMatrix<T>* triangularOther = dynamic_cast<const TriangularMatrix<T>*>(&other);

    if (triangularOther != nullptr && triangularOther->type == type)
    {
        TriangularMatrix<T>* result = new TriangularMatrix<T>(size, type);

        for (int i = 0; i < size; ++i)
        {
            for (int j = GetStoredColumnStart(i); j <= GetStoredColumnEnd(i); ++j)
            {
                result->Set(i, j, Get(i, j) + triangularOther->Get(i, j));
            }
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
Matrix<T>* TriangularMatrix<T>::MultiplyByScalar(const T& scalar) const
{
    TriangularMatrix<T>* result = new TriangularMatrix<T>(size, type);

    for (int i = 0; i < size; ++i)
    {
        for (int j = GetStoredColumnStart(i); j <= GetStoredColumnEnd(i); ++j)
        {
            result->Set(i, j, Get(i, j) * scalar);
        }
    }

    return result;
}

template<typename T>
double TriangularMatrix<T>::Norm() const
{
    double sum = 0.0;

    for (int i = 0; i < size; ++i)
    {
        for (int j = GetStoredColumnStart(i); j <= GetStoredColumnEnd(i); ++j)
        {
            sum += ScalarTraits<T>::MagnitudeSquared(Get(i, j));
        }
    }

    return std::sqrt(sum);
}

template<typename T>
void TriangularMatrix<T>::SwapRows(int firstRow, int secondRow)
{
    CheckSingleIndex(firstRow);
    CheckSingleIndex(secondRow);

    if (firstRow != secondRow)
    {
        throw std::invalid_argument("TriangularMatrix::SwapRows: operation breaks triangular structure");
    }
}

template<typename T>
void TriangularMatrix<T>::SwapColumns(int firstColumn, int secondColumn)
{
    CheckSingleIndex(firstColumn);
    CheckSingleIndex(secondColumn);

    if (firstColumn != secondColumn)
    {
        throw std::invalid_argument("TriangularMatrix::SwapColumns: operation breaks triangular structure");
    }
}

template<typename T>
void TriangularMatrix<T>::MultiplyRow(int row, const T& scalar)
{
    CheckSingleIndex(row);

    for (int j = GetStoredColumnStart(row); j <= GetStoredColumnEnd(row); ++j)
    {
        Set(row, j, Get(row, j) * scalar);
    }
}

template<typename T>
void TriangularMatrix<T>::MultiplyColumn(int column, const T& scalar)
{
    CheckSingleIndex(column);

    for (int i = 0; i < size; ++i)
    {
        if (IsStoredPosition(i, column))
        {
            Set(i, column, Get(i, column) * scalar);
        }
    }
}

template<typename T>
void TriangularMatrix<T>::AddRowMultiple(int sourceRow, int targetRow, const T& scalar)
{
    CheckSingleIndex(sourceRow);
    CheckSingleIndex(targetRow);

    bool preservesStructure =
        (type == TriangularType::Upper && sourceRow >= targetRow) ||
        (type == TriangularType::Lower && sourceRow <= targetRow);

    if (!preservesStructure)
    {
        throw std::invalid_argument("TriangularMatrix::AddRowMultiple: operation breaks triangular structure");
    }

    for (int j = GetStoredColumnStart(targetRow); j <= GetStoredColumnEnd(targetRow); ++j)
    {
        Set(targetRow, j, Get(targetRow, j) + Get(sourceRow, j) * scalar);
    }
}

template<typename T>
void TriangularMatrix<T>::AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar)
{
    CheckSingleIndex(sourceColumn);
    CheckSingleIndex(targetColumn);

    bool preservesStructure =
        (type == TriangularType::Upper && sourceColumn <= targetColumn) ||
        (type == TriangularType::Lower && sourceColumn >= targetColumn);

    if (!preservesStructure)
    {
        throw std::invalid_argument("TriangularMatrix::AddColumnMultiple: operation breaks triangular structure");
    }

    for (int i = 0; i < size; ++i)
    {
        if (IsStoredPosition(i, targetColumn))
        {
            Set(i, targetColumn, Get(i, targetColumn) + Get(i, sourceColumn) * scalar);
        }
    }
}

#endif
