#include "neuron.h"
#include <math.h>

Neuron::Neuron(data_Net data, int Func)
{
    actFunc=new ActivateFunc(Func);
    if(data.layers!=0)
    {
    layers=data.layers;
    n=data.layers;
    }
    else
    {
        layers=data.n;
        n=data.n;
    }

    size=new int[layers];
    for (int i=0; i<layers; i++)
        size[i]=data.size[i];

    weights= new Matrix[layers-1];
//    bios=new Matrix[layers-1];
//    neuronsVal=new Matrix[layers];
//    neuronsErr=new Matrix[layers];
//    neuronBiosVal=new Matrix[layers-1];
//    neuronBiosErr=new Matrix[layers-1];
    for(int i=0;i<layers-1;i++)
    {
        weights[i]=Matrix(size[i+1],size[i]);
//        bios[i]=Matrix(size[i+1]).Rand();
//        neuronsVal[i]=Matrix(size[i]);
//        neuronsErr[i]=Matrix(size[i]);
//        neuronBiosVal[i]=Matrix(1).Rand();
//        neuronBiosErr[i]=Matrix(1).Rand();
    }

    bios = new double* [n - 1];
        for (int i = 0; i < n - 1; i++) {
            bios[i] = new double[size[i + 1]];

            for (int j = 0; j < size[i + 1]; j++) {
                bios[i][j] = ((rand() % 50)) * 0.06 / (size[i] + 15);
            }
        }
        neurons_val = new double* [n]; neurons_err = new double* [n];
        for (int i = 0; i < n; i++) {
            neurons_val[i] = new double[size[i]]; neurons_err[i] = new double[size[i]];
        }
        neurons_bios_val = new double[n - 1]; neurons_bios_err = new double[n - 1];
}

void Neuron::PrintConfig()
{
    cout<<endl<<"======================================="<<endl;
    cout<<"Net has "<< layers<<" Layers"<<endl<<"size[]"<<endl;
    for(int i=0; i<layers;i++)
        cout<<size[i]<<" ";
    cout<<endl<<"======================================="<<endl;
}

void Neuron::SetInputs(double* value)
{
    for(int i=0;i<size[0];i++)
        neurons_val[0][i] = value[i];
}

double Neuron::ForwardFeed()
{
    for (int k = 1; k < n; ++k) {
        Multi(weights[k - 1], neurons_val[k - 1],neurons_val[k]);
        Summ(neurons_val[k], bios[k - 1], size[k]);
        actFunc->use(neurons_val[k], size[k]);
    }
    int pred = SearchMaxIndex(neurons_val[n - 1]);
    return pred;
}

int Neuron::SearchMaxIndex(double* value)
{
    double max = value[0];
    int prediction = 0;
    double tmp;
    for (int j = 1; j < size[n - 1]; j++) {
        tmp = value[j];
        if (tmp > max) {
            prediction = j;
            max = tmp;
        }
    }
    return prediction;
}

void Neuron::BackPropogation(double expect)
{

    for (int i = 0; i < size[n - 1]; i++) {
        if (i != int(expect))
            neurons_err[n - 1][i] = -pow(neurons_val[n - 1][i], 2);
        else
            neurons_err[n - 1][i] = pow(1.0 - neurons_val[n - 1][i], 2);
    }
    for (int k = n - 2; k > 0; k--) {
        Multi_T(weights[k], neurons_err[k + 1], neurons_err[k]);
    }
    for (int i = n - 2; i >= 0; i--) {
        double err = 0;
        for (int j = 0; j < size[i + 1]; j++) {
            err += neurons_err[i + 1][j] * bios[i][j];
        }
        neurons_bios_err[i] = err;
    }

}

double Neuron::ErrorCounter() {
    double err = 0;
    for (int i = 0; i < size[n - 1]; i++) {
        //err += neurons[n - 1][i].error;
    }
    return err;
}

void Neuron::PrintValues()
{

    for (int j = 0; j < size[n - 1]; j++) {
        cout << j << " " << neurons_val[n - 1][j] << endl;
    }
}

void Neuron::WeightsUpdater(double lr) {

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < size[i + 1]; ++j) {
            for (int k = 0; k < size[i]; ++k) {
                weights[i](j, k) += neurons_err[i + 1][j] * actFunc->useDer(neurons_val[i + 1][j]) * neurons_val[i][k] * lr;
            }
        }
    }

    for (int i = 0; i < n - 1; i++) {
        for (int k = 0; k < size[i + 1]; k++) {
            bios[i][k] += neurons_err[i + 1][k] * actFunc->useDer(neurons_val[i + 1][k]) * lr;
        }
    }

}
void Neuron::SaveNet() {
    ofstream fout;
    fout.open("C:\\Project421\\Ekaterina\\Weights.txt");
    if (!fout.is_open()) {
        cout << "Error reading the file";
        system("pause");
    }
    for (int i = 0; i < layers - 1; ++i)
        fout << weights[i] << " ";

    for (int i = 0; i < layers - 1; ++i) {
        for (int j = 0; j < size[i + 1]; ++j) {
            fout << bios[i][j] << " ";
        }
    }
    cout << "Weights saved \n";
    fout.close();
}
void Neuron::LoadNet() {

    ifstream fin;
    fin.open("C:\\Project421\\Ekaterina\\Weights.txt");
    if (!fin.is_open()) {
        cout << "Error reading the file";
        system("pause");
    }
    for (int i = 0; i < n - 1; ++i) {
        fin >> weights[i];
    }
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < size[i + 1]; ++j) {
            fin >> bios[i][j];
        }
    }
    cout << "Weights readed \n";
    fin.close();
}

void Neuron::Multi(const Matrix &m1, const double *neuron,double *c)
{
    //#pragma omp parallel for
    for (int i = 0; i < m1.GetRow(); ++i) {
        double tmp = 0;
        for (int j = 0; j < m1.GetColumn(); ++j) {
            tmp += m1(i,j) * neuron[j];
        }
        c[i] = tmp;
    }
}
void Neuron::Multi_T(const Matrix &m1, const double *neuron, double *c)
{
    //#pragma omp parallel for
    for (int i = 0; i < m1.GetColumn(); ++i) {
        double tmp = 0;
        for (int j = 0; j < m1.GetRow(); ++j) {
            tmp += m1(j,i) * neuron[j];
        }
        c[i] = tmp;
    }

}

void Neuron::Summ(double* a, const double* b, int n)
{
    for (int i = 0; i < n; i++) {
        a[i] += b[i];
    }
}
