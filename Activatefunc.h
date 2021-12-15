#ifndef ACTIVATEFUNC_H
#define ACTIVATEFUNC_H

enum activateFunc{sigmoid, thx};
class ActivateFunc
{
    activateFunc actFun;
public:
    ActivateFunc();
    ActivateFunc(int func);
    //void set(int func);
    void use(double *value, int n);
    double use(double value);

    double useDer(double value);
    void useDer(double* value, int i);

};

#endif // ACTIVATEFUNC_H
