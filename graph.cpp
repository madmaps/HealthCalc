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

    float weightRange = maxWeight - minWeight;
    if(weightRange < 5)
    {
        weightRange = 5;
    }

    highWeight = maxWeight + (weightRange * 0.20);
    lowWeight = maxWeight - weightRange - (weightRange * 0.30);
    startDate = listOfEntries->at(0).getDateTime();
    startDate = QDateTime(QDate(startDate.date().year(),startDate.date().month(),startDate.date().day()));
    qint64 secondsFromBegToEnd = listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - listOfEntries->at(0).getDateTime().toSecsSinceEpoch();

    // If less than 5 days make it 5 days
    if(secondsFromBegToEnd < 432000)
    {
        secondsFromBegToEnd = 432000;
    }
    endDate = QDateTime::fromSecsSinceEpoch(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() + (secondsFromBegToEnd * 0.30));
    endDate = QDateTime(QDate(endDate.date().year(),endDate.date().month(),endDate.date().day()));
    endDate = QDateTime(endDate.date().addDays(1));

}

void Graph::paintEvent(QPaintEvent*)
{
    const int horizontalBorder = 35;
    const int verticalBorder = 60;
    const int gridSize = 20;
    const int labelHeight = 20;

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

    // Draw Weight Points;
    thePen.setWidth(5);
    theColor.setRgb(255,0,0);
    thePen.setColor(theColor);
    secondPen.setColor(theColor);
    secondPen.setWidth(1);
    float weightPercent;
    float datePercent;
    int weightX;
    int weightY;
    int oldWeightX;
    int oldWeightY;
    qint64 timeOne, timeTwo;
    i = 0;
    while(i < (int)listOfEntries->size())
    {
        if(i > 0)
        {
            oldWeightX = weightX;
            oldWeightY = weightY;
        }
        weightPercent = (listOfEntries->at(i).getWeight() - lowWeight) / (highWeight - lowWeight);
        timeOne = listOfEntries->at(i).getDateTime().toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
        timeTwo = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
        datePercent = (float)timeOne/(float)timeTwo;
        weightX = horizontalBorder + ((width - horizontalBorder) * datePercent);
        weightY = (height - verticalBorder) - ((height - verticalBorder) * weightPercent);
        painter.setPen(thePen);
        painter.drawPoint(weightX,weightY);
        if(i > 0)
        {
            painter.setPen(secondPen);
            painter.drawLine(oldWeightX, oldWeightY, weightX, weightY);
        }

        i++;
    }

    //Draw date lines
    theColor.setRgb(22,45,245);
    thePen.setColor(theColor);
    thePen.setWidth(1);
    thePen.setStyle(Qt::SolidLine);
    secondPen.setStyle(Qt::DotLine);
    secondPen.setWidth(5);
    theColor.setRgb(216,220,254);
    secondPen.setColor(theColor);
    painter.setPen(thePen);
    unsigned int numberOfDateLabels = floor((float)(width - horizontalBorder) / (float)labelHeight);
    qint64 secondsBetweenStartAndEnd = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
    unsigned int numberOfDaysBetweenStartAndEnd = floor((float)secondsBetweenStartAndEnd / 86400);
    unsigned int dayDivisor = ceil((float)numberOfDaysBetweenStartAndEnd / (float)numberOfDateLabels);
    unsigned int numberOfLabels = floor((float)numberOfDaysBetweenStartAndEnd / (float)dayDivisor);
    QDateTime currentDate;
    int dateX;

    i = 0;
    while((unsigned int)i < numberOfLabels)
    {
        currentDate = startDate;
        currentDate = QDateTime(currentDate.date().addDays(dayDivisor * i));
        timeOne = currentDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
        timeTwo = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
        datePercent = (float)timeOne/(float)timeTwo;
        dateX = horizontalBorder + ((width - horizontalBorder) * datePercent);
        painter.setPen(secondPen);
        painter.drawLine(dateX,height - verticalBorder,dateX, 0);
        painter.setPen(thePen);
        painter.drawLine(dateX,height - verticalBorder,dateX, 0);
        painter.rotate(90);
        painter.drawText(height - verticalBorder + 5,-dateX + 3,currentDate.toString("MM/dd/yy"));//dateX,height - 15,currentDate.toString("MM/dd/yy"));
        painter.rotate(-90);
        i++;
    }


    //Redraw black border
    thePen.setWidth(1);
    thePen.setStyle(Qt::SolidLine);
    theBrush.setStyle(Qt::NoBrush);
    theColor.setRgb(255,255,255);
    theBrush.setColor(theColor);
    theColor.setRgb(0,0,0);
    thePen.setColor(theColor);
    painter.setPen(thePen);
    painter.setBrush(theBrush);
    painter.drawRect(0,0,width-1,height-1);
}
