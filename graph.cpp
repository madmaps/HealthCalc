#include "graph.h"
#include <QPainter>

Graph::Graph(QWidget* parent) : QLabel(parent)
{

}


void Graph::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    int width = this->width();
    int height = this->height();

    QPen thePen;
    QBrush theBrush;
    QColor theColor;

    theBrush.setStyle(Qt::SolidPattern);

    theColor.setRgb(255,255,255);
    theBrush.setColor(theColor);
    theColor.setRgb(0,0,0);
    thePen.setColor(theColor);
    painter.setPen(thePen);
    painter.setBrush(theBrush);


    painter.drawRect(0,0,width-1,height-1);
}
