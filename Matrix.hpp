#ifndef MATRIX_HPP
#define MATRIX_HPP

template<typename T>
class Matrix
{
public:
    virtual ~Matrix() = default;

    virtual int GetRowCount() const = 0;
    virtual int GetColumnCount() const = 0;

    virtual const T& Get(int row, int column) const = 0;

    virtual void Set(int row, int column, const T& value) = 0;

    virtual Matrix<T>* Clone() const = 0;
    virtual Matrix<T>* Add(const Matrix<T>& other) const = 0;
    virtual Matrix<T>* MultiplyByScalar(const T& scalar) const = 0;
    virtual double Norm() const = 0;

    virtual void SwapRows(int firstRow, int secondRow) = 0;
    virtual void SwapColumns(int firstColumn, int secondColumn) = 0;
    virtual void MultiplyRow(int row, const T& scalar) = 0;
    virtual void MultiplyColumn(int column, const T& scalar) = 0;
    virtual void AddRowMultiple(int sourceRow, int targetRow, const T& scalar) = 0;
    virtual void AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar) = 0;
};

#endif
