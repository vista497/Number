QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG+=console


QMAKE_CXXFLAGS += -fopenmp
QMAKE_CFLAGS += -fopenmp
LIBS += -fopenmp
LIBS += \


INCLUDEPATH+= C:\QtCreator\Tools\mingw900_64\lib\gcc\x86_64-w64-mingw32\11.2.0\include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Activatefunc.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    neuron.cpp \
    numbernet.cpp \
    paint.cpp

HEADERS += \
    Activatefunc.h \
    Matrix.h \
    Timer.h \
    mainwindow.h \
    neuron.h \
    numbernet.h \
    paint.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    numbernet.ui \
    paint.ui
