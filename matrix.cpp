#include "Matrix.h"
#include <thread>
#include <atomic>
#include <assert.h>
#include <math.h>
#include <omp.h>

Matrix::Matrix()
{
    col=row=0;

}

Matrix::Matrix(int _col)
{
    row=1;
    col=_col;

    value=new double*[row];
    for (int i=0; i<row; i++)
        value[i]=new double[col];

    for (int i=0; i<row;i++)
        for(int j=0; j<col;j++)
            value[i][j]=0;
}

Matrix::~Matrix()
{
    if (col>0)
    {
        for(int i=0;i<row;i++)
            delete[] value[i];
    }

    if (row>0)
    {
        delete[] value;
    }
}


Matrix::Matrix(int _row, int _col)
{
    row=_row;
    col=_col;

    value=new double*[row];
    for (int i=0; i<row; i++)
        value[i]=new double[col];

    for (int i=0; i<row;i++)
        for(int j=0; j<col;j++)
            value[i][j]=0;
}

Matrix::Matrix(const Matrix&_M)
{
    col=_M.col;
    row=_M.row;

    value=new double*[row];
    for (int i=0; i<row; i++)
        value[i]=new double[col];

    for (int i=0; i<row;i++)
        for(int j=0; j<col;j++)
            value[i][j]=_M.value[i][j];
}


double Matrix::Get(int _row, int _col)
{
    if ((col>0) && (row>0))
        return value[_row][_col];
    else
        return value[_row][_col];
}

void Matrix::Set(int _row, int _col, double value)
{
    assert(_row>=0 && _row<row);
    assert(_col>=0 && _col<col);

    this->value[_row][_col]=value;
}

void Matrix::Print()
{
    for (int i=0; i<row;i++)
    {
        for(int j=0; j<col;j++)
        {
            cout<<value[i][j]<<"\t";
        }
         cout<< endl;
    }
}

int Matrix::GetRow() const
{
    return this->row;
}

int Matrix::GetColumn() const
{
    return this->col;
}

const Matrix Matrix::T() const
{
    Matrix temp(this->col, this->row);
    for(int i=0; i<this->col; i++)
        for(int j=0;j<this->row; j++)
            temp.value[i][j]=value[j][i];
    return temp;
}

//обратная матрица
const Matrix Matrix::T_1() const
{
    double det = this->Det();
    assert(det!=0 && "det = 0");

    int m = this->row - 1;
    Matrix temp(m,m);
    Matrix obr(this->row,this->col);
    for(int i = 0; i < this->row; i++)
        for(int j = 0; j < this->col; j++){
            temp=Minor(j, i, *this);
            obr(i,j)=pow(-1.0, i + j + 2) * temp.Det() / det;
        }
    return obr;
}

//определитель
double Matrix :: Det() const
{
    assert(this->col==this->row);

    if(this->col==0) return 0;
    if(this->col == 1) return this->value[0][0];
    if (this->col==2) return this->value[0][0] * this->value[1][1]-(this->value[1][0] * this->value[0][1]);

    int l;
    double d;
    double sum11=1,sum12=0, sum21=1, sum22=0;
    int razm=this->row;
    // находим детерминант
            for (int i=0;i<razm;i++)
            {
                    sum11=1; l=2*razm-1-i;sum21=1;
            for (int j=0;j<razm;j++)
                    {
                sum21*=this->value[j][l%razm];
                l--;
                sum11*=this->value[j][(j+i)%(razm)];
            }
            sum22+=sum21;
            sum12+=sum11;
        }
        d=sum12-sum22;
    return d;
}

Matrix Matrix::Minor(int iRow, int jCol, const Matrix &M)const
{
    assert(this->row==this->col && this->col!=0&&this->row!=0);

    int ki = 0;
    Matrix temp(M.row-1,M.col-1);
    for (int i = 0; i < M.row; i++)
        if(i != iRow){
            for (int j = 0, kj = 0; j < M.col; j++){
                if (j != jCol){
                    temp(ki,kj) = M.value[i][j];
                    kj++;
                }
            }
            ki++;
        }
    return temp;
}


const Matrix Matrix::operator-(const Matrix &_M) const
{
    assert(_M.col==this->col);
    assert(_M.row==this->row);

    Matrix temp(_M.row,_M.col);
    for(int i=0; i<_M.row; i++)
        for(int j=0;j<_M.col; j++)
            temp.value[i][j]=value[i][j]-_M.value[i][j];
    return temp;
}

const Matrix Matrix::operator-(const double &x) const
{
    assert(this->row==this->col);

    Matrix tempX(this->row, this->col);
    for(int i=0;i<this->row;i++)
        for(int j=0;j<this->col;j++)
        {
            if(i==j)
                tempX(i,j)=x;
            tempX(i,j)=0;
        }

    Matrix temp(this->row, this->col);
    for(int i=0;i<this->row;i++)
        for(int j=0;j<this->col;j++)
            temp(i,j)=this->value[i][j]-tempX(i,j);
    return temp;
}

const Matrix Matrix::operator+(const Matrix &_M) const
{
    assert(_M.col==this->col);
    assert(_M.row==this->row);

    Matrix temp(_M.row,_M.col);
    for(int i=0; i<_M.col; i++)
        for(int j=0;j<_M.row; j++)
            temp.value[i][j]=value[i][j]+_M.value[i][j];
    return temp;
}

const Matrix Matrix::operator+(const double &x) const
{
    assert(this->row==this->col);

    Matrix tempX(this->row, this->col);
    for(int i=0;i<this->row;i++)
        for(int j=0;j<this->col;j++)
        {
            if(i==j)
                tempX(i,j)=x;
            tempX(i,j)=0;
        }

    Matrix temp(this->row, this->col);
    for(int i=0;i<this->row;i++)
        for(int j=0;j<this->col;j++)
            temp(i,j)=this->value[i][j]+tempX(i,j);
    return temp;
}

const Matrix& Matrix::operator=(const Matrix &_M)
{
    //����������� ������, ���������� ����� ��� ������� *this
    if (col>0)
    {
        for(int i=0;i<row;i++)
            delete[] value[i];
    }

    if (row>0)
    {
        delete[] value;
    }

    col=_M.col;
    row=_M.row;

    value= new double*[row];
    for (int i=0; i<row; i++)
        value[i]=new double[col];

    for (int i=0; i<row;i++)
        for(int j=0; j<col;j++)
            value[i][j]=_M.value[i][j];
    return *this;
}

const Matrix Matrix::operator*(const double &x) const
{
    Matrix temp=*this;
   #pragma omp parallel for
    for(int i=0; i<this->row; i++)
        for(int j=0;j<this->col; j++)
                temp.value[i][j]=value[i][j]*x;
    return temp;
}

const Matrix Matrix::operator*(const Matrix &_M)
{
    assert(this->col==_M.row);


    Matrix temp(_M.row,this->col);
    Matrix MT=_M.T();
    int i,j,k=0;

    #pragma omp parallel for
    for(i=0; i<this->row; i++)
    {
        for(j=0;j<_M.col; j++)
        {
            for(k=0; k<_M.row; k++)
            {
                temp.value[i][j]+=this->value[i][k]*MT.value[j][k];
            }
        }
    }
    return temp;
}

const Vector Matrix::operator*(const Vector *V) const
{
    assert(this->col == V->RowCount());

    Vector temp(this->row);
            for (int j = 0; j < this->col; j++)
                    for (int i = 0; i < this->row; i++)
                            temp(j) = temp(j) + value[i][j]*V->operator()(i);


    return temp;
}

ostream &operator<<(ostream &output, const Matrix &_M)
{
    for (int i=0; i<_M.row;i++)
    {
        for(int j=0;j<_M.col;j++)
        {
            output << _M.value[i][j] << "\t";
        }
        output<<endl;
    }
    return output;
}

istream& operator >> (istream& is, Matrix& m)
{
    for (int i = 0; i < m.row; ++i) {
        for (int j = 0; j < m.col; j++) {
            is >> m.value[i][j];
        }
    }
    return is;
}

int Matrix::GetProcessorCount() const
{
    const auto processor_count = std::thread::hardware_concurrency();
    return processor_count;
}

double &Matrix::operator()(const int i, const int j) const
{
    assert(i >= 0 && i < this->row);
    assert(j >= 0 && j < this->col);

    return this->value[i][j];
}

Matrix Matrix::Rand(int min, int max)
{
    srand(randX*time(0));
    randX*=14;
    int precision=4;

    for (int i=0; i<row; i++)

        for(int j=0;j<col;j++)
        {
        double v = rand() % (int)pow(10, precision);
        v=min + (v / pow(10, precision)) * (max - min);
        this->value[i][j]=v;
        }
    return *this;
}



Matrix GetRowVector(int col)
{
    Matrix temp(0,col);

}

Matrix GetColVector(int row)
{
    Matrix temp(row,0);
}

//====Vector====================================================================
//кол-во строк
int Vector::RowCount() const
{
        return row;
}

double &Vector::operator()(const int j) const
{
    //assert(i >= 0 && i < this->row);
    assert(j >= 0 && j < this->col);

    return this->value[0][j];
}

//оператор присваивания
const Vector &Vector::operator =(const Vector &V)
{
        if(&V != this)
        {
            assert(V.col==this->col);
            assert(V.row==this->row);
                        delete [] this->value;
                        value = new double* [V.row];
                        if (!value)
                        {
                                cout << "Error in Vector memory\n";
                                system("pause");
                                exit(1);
                        }
                }
                for (int j = 0; j < V.RowCount(); j++)
                        value[j][0] = V.value[j][0];
                row = V.RowCount();
                col = 1; //это вектор!

        return *this;
}

//сумма с др вектором-столбцом
const Vector Vector::operator +(const Vector &V2) const
{
    assert(V2.col==this->col);
    assert(V2.row==this->row);

        Vector temp(this->row);
        for (int j = 0; j < this->row; j++)
                temp.value[j][0] = this->value[j][0] + V2.value[j][0];
        return temp;
}

//разность с др вектором-столбцом
const Vector Vector::operator -(const Vector &V2) const
{
    assert(V2.col==this->col);
    assert(V2.row==this->row);

        Vector temp(this->row);
        for (int j = 0; j < this->row; j++)
                temp.value[j][0] = this->value[j][0] - V2.value[j][0];
        return temp;
}

//умножение вектора-столбца на число
const Vector Vector::operator *(double real) const
{
        Vector temp(this->row);
        for (int j = 0; j < this->row; j++)
                temp.value[j][0] = this->value[j][0] * real;
        return temp;
}

//дружественная фун-ия. умножение числа на вектор-столбец (именно такой порядок! число * вектор)
const Vector operator *(const double real, const Vector &V1)
{
        Vector temp(V1.RowCount());
        for (int j = 0; j < temp.RowCount(); j++)
                temp.value[j][0] = V1.value[j][0]*real;
        return temp;
}

//умножение вектора на вектор (векторное произведение)
const Vector Vector::operator *(const Vector V2) const
{
        Vector temp(this->row);
//        temp[0] = this->operator [](1)*V2.mas[0][2] - this->operator [](2)*V2.mas[0][1];
//        temp[1] = this->operator [](2)*V2.mas[0][0] - this->operator [](0)*V2.mas[0][2];
//        temp[2] = this->operator [](0)*V2.mas[0][1] - this->operator [](1)*V2.mas[0][0];
        for(int i=0;i<this->row;i++)
        {
//            temp.value[i][0]=this->value[i][0]-
        }
        return temp;
}

//модуль вектора
double Vector::Module() const
{
        double temp = 0;
        for(int j = 0; j < this->row; j++)
                temp = temp + this->value[j][0]*this->value[j][0];
        temp = sqrt(temp);
        return temp;
}

Vector Vector::Rand(int min, int max)
{
    srand(randX*time(0));
    randX*=14;
    int precision=4;

        for(int j=0;j<this->RowCount();j++)
        {
        double v = rand() % (int)pow(10, precision);
        v=min + (v / pow(10, precision)) * (max - min);
        this->value[j][0]=v;
        }
    return *this;
}

