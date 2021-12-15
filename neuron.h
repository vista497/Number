#ifndef NUERON_H
#define NUERON_H
#include "Activatefunc.h"
#include "Matrix.h"
#include <fstream>
using namespace std;

//для сравнения сетей с разной конфигурацией
struct data_Net
{
    int layers;
    int n=layers;
    int* size;
};



class Neuron
{
    int layers;             //кол-во слоев
    int n;
    int *size;              //кол-во нейронов на каждом слое
    ActivateFunc *actFunc;
    Matrix *weights;        //матрица весов
    double **bios;          //веса смещения
    double **neurons_val, **neurons_err; //значения нейронов, ошибки нейронов
    double *neurons_bios_val, *neurons_bios_err;  //значение нейрона смещения

public:
    Neuron(data_Net date, int actFunc);
    void PrintConfig();
    void SetInputs(double* value);
    void WeightsUpdater(double lr);
    void SaveNet();
    void LoadNet();

    double ForwardFeed();
    void BackPropogation(double expect);
    int SearchMaxIndex(double* value);
    void PrintValues();
    double ErrorCounter();

    void Multi(const Matrix &M, const double* M2, double *c);
    void Multi_T(const Matrix &M, const double* M2,double *c);
    void Summ(double* a, const double* b, int n);
};

#endif // NUERON_H
