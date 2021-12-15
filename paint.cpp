#include "paint.h"
#include "ui_paint.h"
#include <iostream>


Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    draw = false;
}

Paint::~Paint()
{
    delete ui;
    setFocusPolicy(Qt::StrongFocus);
    draw = false;
}

//событие перерисовки виджета
void Paint::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPalette Pal(palette()); // содержит группы цветов для каждого состояния виджета.
    Pal.setColor(QPalette::Window,Qt::black); //устанавливаем цвет
    setAutoFillBackground(true); //Это свойство определяет, заполняется ли фон виджета автоматически
    setPalette(Pal); //приминяет выводимый цвет
    painter.setRenderHint(QPainter::Antialiasing, true); //задает качество
    painter.setPen(QPen(Qt::green, 10, Qt::SolidLine));  //ставим ручку и задаем цвет и радиус
    for (int i = 0; i < vv.size(); ++i)
        if (i > 0) painter.drawEllipse(vv[i - 1], 12, 12); // отрисовываем
}

//событие нажатия кнопки мышки
void Paint:: mousePressEvent(QMouseEvent * event)
{
    draw=true;
}

//событие перемещения мыши с зажатой кнопкой
void Paint:: mouseMoveEvent(QMouseEvent *event)
{
    //координаты
    if (draw) {
        vv.push_back(event->pos());
    }
    update();
}

void Paint::clear()
{
    vv.clear();
    update();
}

//событие отпускания кнопки мышки
void Paint::mouseReleaseEvent(QMouseEvent * event)
{
    draw = false;
    QPixmap scr; //для работы с изображением
    scr =  this->grab(); //Отображает виджет в виде растрового изображения

    QFile mFile("C:\\Project421\\Ekaterina\\test.txt");
    mFile.open(QIODevice::WriteOnly); //открываем только для записи
    QTextStream fin(&mFile); //интерфейс для чтения и записи текста
    fin.setRealNumberPrecision(2); //значение описывает количество дробных цифр

    QImage img;
    img = scr.toImage(); //обраное преобразование растрового изображения
    img = img.scaled(28, 28); //Возвращает копию изображения, масштабированную до прямоугольника с заданной шириной и высотой
    int w = img.size().width();
    int h = img.size().height();

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            fin << img.pixelColor(j, i).green() / 255. << " ";



        }
        fin << Qt::endl;
    }
    mFile.close();

}
