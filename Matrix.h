#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <iomanip>

using namespace std;

class Vector;

class Matrix
{
protected:

    int row; // строки
    int col; // столбцы
    double **value;
    int GetProcessorCount() const ;
    double Len();
    int randX=1;

public:
    Matrix(); //
    Matrix(int len);
    Matrix(int _m, int _row); 	// конструктор с параметрами (нулевая матрица)
    Matrix(const Matrix &_M); 	// конструктор копирования

    ~Matrix();

    double Get(int _m, int _row);
    void Set(int _m, int _row, double _value);
    void Print();
    int GetRow() const; 			//длина
    int GetColumn() const;
    Matrix Rand(int min=0, int max=1);

    Matrix GetRowVector(int col);
    Matrix GetColVector(int row);

    const Matrix T() const;
    const Matrix T_1() const;
    double Det() const;
    Matrix Minor(int i, int j, const Matrix &M)const;


    const Matrix operator-(const Matrix &_M) const;
    const Matrix operator-(const double &x) const;
    const Matrix operator+(const Matrix &_M) const;
    const Matrix operator+(const double &x) const;
    const Matrix &operator=(const Matrix &_M);
    const Matrix &operator=(const double &x);
    const Matrix operator*(const double &x) const;
    const Matrix operator*(const Matrix &_M);
    const Vector operator*(const Vector *V) const;
    double& operator()(const int i, const int j) const;

    friend ostream &operator<< (ostream &output, const Matrix &_M); //оператор вывода
    friend istream& operator >> (istream& is, Matrix& m);
};

//класс вектор-столбца родитель матрица
class Vector: protected Matrix {
public:
        Vector Rand(int min=0, int max=1);
        int RowCount() const;     //кол-во ячеек
        Vector(int col) : Matrix(col){}; //конструктор
        Vector(const Vector &ob){
            col=ob.col;
            row=ob.row;

            value=new double*[row];
            for (int i=0; i<row; i++)
                value[i]=new double[col];

            for (int i=0; i<row;i++)
                for(int j=0; j<col;j++)
                    value[i][j]=ob.value[i][j];
        };  //конструктор копий
        const Vector &operator =(const Vector &V2);        //приравнять к др вектору. перегружена
        const Vector operator +(const Vector &V2) const;   //сумма с др вектором
        const Vector operator -(const Vector &V2) const;   //разность с др вектором
        const Vector operator *(double real) const;        //умножение вектора на число
        friend const Vector operator *(const double real, const Vector &V1);  //умножение числа на вектор (именно такой порядок! число * вектор)
        const Vector operator *(const Vector V2) const;       //умножение вектора на вектор размерами 3 на 3! (векторное произведение)
        double Module() const;                           //модуль вектора
        double& operator()(const int i) const;
};

#endif
