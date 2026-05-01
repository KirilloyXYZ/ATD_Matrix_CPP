#ifndef SQUAREMATRIX_HPP
#define SQUAREMATRIX_HPP

#include <stdexcept>

#include "RectangularMatrix.hpp"

template<typename T>
class SquareMatrix : public RectangularMatrix<T>
{
public:
    SquareMatrix();
    explicit SquareMatrix(int size, const T& defaultValue = T());
    SquareMatrix(const T* items, int size);
    SquareMatrix(const SquareMatrix<T>& other);
    SquareMatrix<T>& operator=(const SquareMatrix<T>& other);

    int GetSize() const;

    Matrix<T>* Clone() const override;
    Matrix<T>* Add(const Matrix<T>& other) const override;
    Matrix<T>* MultiplyByScalar(const T& scalar) const override;
};

template<typename T>
SquareMatrix<T>::SquareMatrix() : RectangularMatrix<T>(0, 0) { }

template<typename T>
SquareMatrix<T>::SquareMatrix(int size, const T& defaultValue) : RectangularMatrix<T>(size, size, defaultValue) { }

template<typename T>
SquareMatrix<T>::SquareMatrix(const T* items, int size) : RectangularMatrix<T>(items, size, size) { }

template<typename T>
SquareMatrix<T>::SquareMatrix(const SquareMatrix<T>& other) : RectangularMatrix<T>(other) { }

template<typename T>
SquareMatrix<T>& SquareMatrix<T>::operator=(const SquareMatrix<T>& other)
{
    RectangularMatrix<T>::operator=(other);
    return *this;
}

template<typename T>
int SquareMatrix<T>::GetSize() const
{
    return this->GetRowCount();
}

template<typename T>
Matrix<T>* SquareMatrix<T>::Clone() const
{
    return new SquareMatrix<T>(*this);
}

template<typename T>
Matrix<T>* SquareMatrix<T>::Add(const Matrix<T>& other) const
{
    if (this->GetRowCount() != other.GetRowCount() || this->GetColumnCount() != other.GetColumnCount())
    {
        throw std::invalid_argument("SquareMatrix::Add: matrices must have equal dimensions");
    }

    SquareMatrix<T>* result = new SquareMatrix<T>(GetSize());

    for (int i = 0; i < GetSize(); ++i)
    {
        for (int j = 0; j < GetSize(); ++j)
        {
            result->Set(i, j, this->Get(i, j) + other.Get(i, j));
        }
    }

    return result;
}

template<typename T>
Matrix<T>* SquareMatrix<T>::MultiplyByScalar(const T& scalar) const
{
    SquareMatrix<T>* result = new SquareMatrix<T>(GetSize());

    for (int i = 0; i < GetSize(); ++i)
    {
        for (int j = 0; j < GetSize(); ++j)
        {
            result->Set(i, j, this->Get(i, j) * scalar);
        }
    }

    return result;
}

#endif
