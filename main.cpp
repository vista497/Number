#include "numbernet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NumberNet p;
    p.show();
    return a.exec();
}

