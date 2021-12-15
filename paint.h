#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QPalette>
#include <QMouseEvent>
#include <QFile>
#include <QPalette>
#include <QPainter>

namespace Ui {
class Paint;
}

class Paint : public QWidget
{
    Q_OBJECT

    bool draw;
    QVector <QPointF> vv;
    QImage pic;

public:
    explicit Paint(QWidget *parent = nullptr);
    ~Paint();

    void paintEvent(QPaintEvent*);
   virtual  void mousePressEvent(QMouseEvent*);
   virtual  void mouseMoveEvent(QMouseEvent*);
   virtual  void mouseReleaseEvent(QMouseEvent*);


public slots:
    void clear();

private:
    Ui::Paint *ui;
};

#endif // PAINT_H

