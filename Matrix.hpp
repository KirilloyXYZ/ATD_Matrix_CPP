#ifndef MATRIX_HPP
#define MATRIX_HPP

template<typename T>
class Matrix
{
public:
    virtual ~Matrix() = default;

    virtual int GetRowCount() const = 0; // строки 
    virtual int GetColumnCount() const = 0; // столбцы

    virtual const T& Get(int row, int column) const = 0; // элемент
    virtual void Set(int row, int column, const T& value) = 0;

    virtual Matrix<T>* Clone() const = 0; // копия матрицы в динамической памяти
    virtual Matrix<T>* Add(const Matrix<T>& other) const = 0; // складывает матрицы
    virtual Matrix<T>* MultiplyByScalar(const T& scalar) const = 0;
    virtual double Norm() const = 0; // норма Фробениуса

    virtual void SwapRows(int firstRow, int secondRow) = 0; // меняет местами две строки
    virtual void SwapColumns(int firstColumn, int secondColumn) = 0; // столбцы
    virtual void MultiplyRow(int row, const T& scalar) = 0; // умножает строку на число
    virtual void MultiplyColumn(int column, const T& scalar) = 0; // столбец на число
    virtual void AddRowMultiple(int sourceRow, int targetRow, const T& scalar) = 0; // прибавляет к строке строку умноженную на скаляр
    virtual void AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar) = 0; 
};

#endif
