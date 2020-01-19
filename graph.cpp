#include "graph.h"
#include <QPainter>
#include <math.h>

Graph::Graph(QWidget* parent) : QLabel(parent)
{

}

void Graph::setListOfEntries(std::vector<Entry>* inListOfEntries)
{
    listOfEntries = inListOfEntries;
}

void Graph::updateVariables()
{
    //Finding max weight
    maxWeight = 0;
    for(Entry check : *listOfEntries)
    {
        if(check.getWeight()>maxWeight)
        {
            maxWeight = check.getWeight();
        }
    }

    //Finding min weight
    minWeight = 999999;
    for(Entry check : *listOfEntries)
    {
        if(check.getWeight() < minWeight)
        {
            minWeight = check.getWeight();
        }
    }

}

void Graph::paintEvent(QPaintEvent*)
{
    const int horizontalBorder = 35;
    const int verticalBorder = 35;
    const int gridSize = 20;

    QPainter painter(this);
    int width = this->width();
    int height = this->height();
    QPen thePen;
    QBrush theBrush;
    QColor theColor;

    //Draw white over entire background with black border
    theBrush.setStyle(Qt::SolidPattern);
    theColor.setRgb(255,255,255);
    theBrush.setColor(theColor);
    theColor.setRgb(0,0,0);
    thePen.setColor(theColor);
    painter.setPen(thePen);
    painter.setBrush(theBrush);
    painter.drawRect(0,0,width-1,height-1);

    //Draw light blue over graph
    theColor.setRgb(251,255,254);
    theBrush.setColor(theColor);
    thePen.setColor(theColor);
    painter.setPen(thePen);
    painter.setBrush(theBrush);
    painter.drawRect(horizontalBorder,1,width-38,height-verticalBorder);

    //Draw Grid Lines
    QPen secondPen;
    theColor.setRgb(237,248,254);
    thePen.setColor(theColor);
    theColor.setRgb(213,238,253);
    secondPen.setColor(theColor);
    thePen.setWidth(3);
    int numberOfVerticalLines = floor((float)(height - verticalBorder) / gridSize);
    int i = 0;
    while(i <= numberOfVerticalLines)
    {
        painter.setPen(thePen);
        painter.drawLine(horizontalBorder, (height - verticalBorder) - (i * gridSize) - 1, width - 3, (height - verticalBorder) - (i * gridSize) - 1);
        painter.setPen(secondPen);
        painter.drawLine(horizontalBorder, (height - verticalBorder) - (i * gridSize), width - 3, (height - verticalBorder) - (i * gridSize));
        i++;
    }
    int numberOfHorizontalLines = floor(((float)width - horizontalBorder) / gridSize);
    i = 0;
    {
        while(i <= numberOfHorizontalLines)
        {
            painter.setPen(thePen);
            painter.drawLine(horizontalBorder + (i * gridSize) - 1, 2 + 1, horizontalBorder + (i * gridSize) - 1, height - verticalBorder + 1);
            painter.setPen(secondPen);
            painter.drawLine(horizontalBorder + (i * gridSize) - 1, 2, horizontalBorder + (i * gridSize) - 1, height - verticalBorder);
            i++;
        }
    }


    //Redraw black border
    thePen.setWidth(1);
    theBrush.setStyle(Qt::NoBrush);
    theColor.setRgb(255,255,255);
    theBrush.setColor(theColor);
    theColor.setRgb(0,0,0);
    thePen.setColor(theColor);
    painter.setPen(thePen);
    painter.setBrush(theBrush);
    painter.drawRect(0,0,width-1,height-1);
}
