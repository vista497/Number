#ifndef NUMBERNET_H
#define NUMBERNET_H

#include <QMainWindow>

#include "neuron.h"
#include "paint.h"
#include  <QMessageBox>

namespace Ui {
class NumberNet;
}

struct data_info {
    double* pixels;
    int digit;
};

class NumberNet : public QMainWindow
{
    Q_OBJECT

    Paint PS;
    Neuron *NW;
    data_Net NW_config;


public:
    explicit NumberNet(QWidget *parent = nullptr);
    ~NumberNet();

    data_Net ReadDataNetWork(string path);
    void ReadTest(double* input, int input_n);



signals:
    void endWriteToFile();

public slots:
    void StartGuess();
    void clearLCD();

private slots:

    void on_pushButton_3_clicked();


    void on_comboBox_activated(int index);

private:
    Ui::NumberNet *ui;
};

#endif // NUMBERNET_H


