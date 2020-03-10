#include "graph.h"
#include <QPainter>
#include <math.h>

Graph::Graph(QWidget* parent) : QLabel(parent)
{
    horizontalBorder = 35;
    verticalBorder = 60;
}

void Graph::setListOfEntries(std::vector<Entry>* inListOfEntries)
{
    listOfEntries = inListOfEntries;
}


void Graph::setDataAnalysis(DataAnalysis *inDataAnalysis)
{
    theDataAnalysis = inDataAnalysis;
}

void Graph::updateVariables()
{
    qint64 secondsFromBegToEnd = 0;
    float weightRange = 0;
    std::vector<Entry>::iterator startDateIter, countIter;
    bool complete = false;
    if(listOfEntries->size() > 1)
    {
        startDate = listOfEntries->at(0).getDateTime();
        secondsFromBegToEnd = listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - listOfEntries->at(0).getDateTime().toSecsSinceEpoch();
        switch(currentState)
        {
        case allData:
            startDate = listOfEntries->at(0).getDateTime();
            startDateIter = listOfEntries->begin();
            break;
        case pastWeek:
            if(secondsFromBegToEnd > (60 * 60 * 24 * 7))
            {
                startDate = QDateTime::fromSecsSinceEpoch(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - (60 * 60 * 24 * 7));
            }
            break;
        case pastTwoWeeks:
            if(secondsFromBegToEnd > (60 * 60 * 24 * 14))
            {
                startDate = QDateTime::fromSecsSinceEpoch(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - (60 * 60 * 24 * 14));
            }
            break;
        case pastThreeWeeks:
            if(secondsFromBegToEnd > (60 * 60 * 24 * 21))
            {
                startDate = QDateTime::fromSecsSinceEpoch(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - (60 * 60 * 24 * 21));
            }
            break;
        case pastMonth:
            if(secondsFromBegToEnd > (60 * 60 * 24 * 30))
            {
                startDate = QDateTime::fromSecsSinceEpoch(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - (60 * 60 * 24 * 30));
            }
            break;
        case fullView:
            startDate = listOfEntries->at(0).getDateTime();
            break;
        default:
            startDate = listOfEntries->at(0).getDateTime();
            startDateIter = listOfEntries->begin();
            break;
        }
        complete = false;
        for(countIter = listOfEntries->begin(); countIter < listOfEntries->end() && !complete; countIter++)
        {
            if(countIter->getDateTime().toSecsSinceEpoch() > startDate.toSecsSinceEpoch())
            {
                startDateIter = --countIter;
                complete = true;
            }
        }

        startDate = QDateTime(QDate(startDate.date().year(),startDate.date().month(),startDate.date().day()));
        secondsFromBegToEnd = listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - startDateIter->getDateTime().toSecsSinceEpoch();
        // If less than 5 days make it 5 days
        if(secondsFromBegToEnd < 432000)
        {
            secondsFromBegToEnd = 432000;
        }
        endDate = QDateTime::fromSecsSinceEpoch(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() + (secondsFromBegToEnd * 1.0));
        endDate = QDateTime(QDate(endDate.date().year(),endDate.date().month(),endDate.date().day()));
        endDate = QDateTime(endDate.date().addDays(1));
        maxWeight = 0;
        for(countIter = startDateIter; countIter < listOfEntries->end(); countIter++)
        {
            if(countIter->getWeight() > maxWeight)
            {
                maxWeight = countIter->getWeight();
            }
        }
        minWeight = 9999999;
        for(countIter = startDateIter; countIter < listOfEntries->end(); countIter++)
        {
            if(countIter->getWeight() < minWeight)
            {
                minWeight = countIter->getWeight();
            }
        }
        weightRange = maxWeight - minWeight;
        if(weightRange < 5)
        {
            weightRange = 5;
        }
        highWeight = maxWeight + (weightRange * 0.20);
        lowWeight = maxWeight - weightRange - (weightRange * 1.0);

    }
        /*for(Entry check : *listOfEntries)
        {
            if(check.getWeight()>maxWeight)
            {
                maxWeight = check.getWeight();
            }
        }
        weightRange = maxWeight - minWeight;
        if(weightRange < 5)
        {
            weightRange = 5;
        }
        highWeight = maxWeight + (weightRange * 0.20);
        lowWeight = maxWeight - weightRange - (weightRange * 1.0);
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
        weightRange = maxWeight - minWeight;
        if(weightRange < 5)
        {
            weightRange = 5;
        }
        highWeight = maxWeight + (weightRange * 0.20);
        lowWeight = maxWeight - weightRange - (weightRange * 1.0);








    //Finding max weight
    if(1)//autoWeight)
    {
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
        lowWeight = maxWeight - weightRange - (weightRange * 1.0);
    }
    if(listOfEntries->size() > 0)
    {
        startDate = listOfEntries->at(0).getDateTime();
        startDate = QDateTime(QDate(startDate.date().year(),startDate.date().month(),startDate.date().day()));
        qint64 secondsFromBegToEnd = listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - listOfEntries->at(0).getDateTime().toSecsSinceEpoch();

        // If less than 5 days make it 5 days
        if(secondsFromBegToEnd < 432000)
        {
            secondsFromBegToEnd = 432000;
        }
        endDate = QDateTime::fromSecsSinceEpoch(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() + (secondsFromBegToEnd * 1.0));
        endDate = QDateTime(QDate(endDate.date().year(),endDate.date().month(),endDate.date().day()));
        endDate = QDateTime(endDate.date().addDays(1));
    }
*/
    theDataAnalysis->updateVariables();
}



void Graph::setShowPrediction(const bool& inShowPrediction)
{
    showPrediction = inShowPrediction;
}

void Graph::setShowWeightRange(const bool& inShowWeightRange)
{
    showWeightRange = inShowWeightRange;
}

void Graph::setShowTargetWeight(const bool& inShowTargetWeight)
{
    showTargetWeight = inShowTargetWeight;
}

void Graph::setCurrentState(const graphState &inCurrentState)
{
    currentState = inCurrentState;
}


void Graph::paintEvent(QPaintEvent*)
{
    //const int horizontalBorder = 35;
    //const int verticalBorder = 60;
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
    int weightX = 0;
    int weightY = 0;
    int oldWeightX = 0;
    int oldWeightY = 0;
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

    //Draw Prediction points
    if(theDataAnalysis != 0)
    {
        int upperWeightX, upperWeightY;
        int oldUpperWeightX, oldUpperWeightY;
        QPointF points[4];
        thePen.setWidth(1);
        thePen.setStyle(Qt::NoPen);
        theColor.setRgb(255,150,34,70);
        theBrush.setColor(theColor);
        theBrush.setStyle(Qt::SolidPattern);
        thePen.setColor(theColor);
        theColor.setRgb(255,150,34);
        secondPen.setColor(theColor);
        secondPen.setWidth(5);
        i = 0;
        while(i < (int)theDataAnalysis->getSizeOfBMRData())
        {
            if(i > 0)
            {
                oldWeightX = weightX;
                oldWeightY = weightY;
                oldUpperWeightX = upperWeightX;
                oldUpperWeightY = upperWeightY;
            }
            weightPercent = (theDataAnalysis->getLowerBMRWeightAt(i) - lowWeight) / (highWeight - lowWeight);
            timeOne = theDataAnalysis->getBMRDataSecondsSinceEpoch(i) - startDate.toSecsSinceEpoch();
            timeTwo = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
            datePercent = (float)timeOne/(float)timeTwo;
            weightX = horizontalBorder + ((width - horizontalBorder) * datePercent);
            weightY = (height - verticalBorder) - ((height - verticalBorder) * weightPercent);

            weightPercent = (theDataAnalysis->getUpperBMRWeightAt(i) - lowWeight) / (highWeight - lowWeight);
            timeOne = theDataAnalysis->getBMRDataSecondsSinceEpoch(i) - startDate.toSecsSinceEpoch();
            timeTwo = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
            datePercent = (float)timeOne/(float)timeTwo;
            upperWeightX = horizontalBorder + ((width - horizontalBorder) * datePercent);
            upperWeightY = (height - verticalBorder) - ((height - verticalBorder) * weightPercent);

            if(i > 0 && showWeightRange && listOfEntries->size() > 2)
            {
                painter.setPen(thePen);
                painter.setBrush(theBrush);
                points[0] = QPointF(oldWeightX,oldWeightY);
                points[1] = QPointF(weightX,weightY);
                points[2] = QPointF(upperWeightX,upperWeightY);
                points[3] = QPointF(oldUpperWeightX,oldUpperWeightY);
                painter.drawPolygon(points,4);
                painter.setPen(secondPen);
                painter.drawLine(oldWeightX, oldWeightY, weightX, weightY);
                painter.drawLine(oldUpperWeightX,oldUpperWeightY,upperWeightX,upperWeightY);


            }
            i++;
        }
        //Draw Prediction Lines
        if(listOfEntries->size() > 0)
        {
            thePen.setWidth(1);
            thePen.setStyle(Qt::NoPen);
            theColor.setRgb(255,244,111,180);
            theBrush.setColor(theColor);
            theBrush.setStyle(Qt::SolidPattern);
            thePen.setColor(theColor);
            theColor.setRgb(255,244,111);
            secondPen.setColor(theColor);
            secondPen.setWidth(5);
            QDateTime predictionDateTime = listOfEntries->at(listOfEntries->size()-1).getDateTime();
            float predictionWeight = theDataAnalysis->getLowerBMRWeightAt(theDataAnalysis->getSizeOfBMRData()-1);
            if(theDataAnalysis != 0)
            {
                if(theDataAnalysis->getSizeOfBMRData() > 3 && showPrediction)
                {
                    for(unsigned int i = 0;i <= 365; i++)
                    {
                        if(i > 0)
                        {
                            oldWeightX = weightX;
                            oldWeightY = weightY;
                            oldUpperWeightX = upperWeightX;
                            oldUpperWeightY = upperWeightY;
                        }
                        weightPercent = (predictionWeight - lowWeight) / (highWeight - lowWeight);
                        timeOne = predictionDateTime.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
                        timeTwo = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
                        datePercent = (float)timeOne/(float)timeTwo;
                        weightX = horizontalBorder + ((width - horizontalBorder) * datePercent);
                        weightY = (height - verticalBorder) - ((height - verticalBorder) * weightPercent);

                        weightPercent = (predictionWeight + theDataAnalysis->getWeightRange() - lowWeight) / (highWeight - lowWeight);
                        timeOne = predictionDateTime.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
                        timeTwo = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
                        datePercent = (float)timeOne/(float)timeTwo;
                        upperWeightX = horizontalBorder + ((width - horizontalBorder) * datePercent);
                        upperWeightY = (height - verticalBorder) - ((height - verticalBorder) * weightPercent);

                        if(i > 0)
                        {
                            painter.setPen(thePen);
                            painter.setBrush(theBrush);
                            points[0] = QPointF(oldWeightX,oldWeightY);
                            points[1] = QPointF(weightX,weightY);
                            points[2] = QPointF(upperWeightX,upperWeightY);
                            points[3] = QPointF(oldUpperWeightX,oldUpperWeightY);
                            painter.drawPolygon(points,4);
                            painter.setPen(secondPen);
                            painter.drawLine(oldWeightX, oldWeightY, weightX, weightY);
                            painter.drawLine(oldUpperWeightX,oldUpperWeightY,upperWeightX,upperWeightY);


                        }
                        predictionWeight = theDataAnalysis->getPredictedWeight(predictionDateTime,predictionWeight,theDataAnalysis->getAverageCaloriesPerDay());
                        predictionDateTime = QDateTime::fromSecsSinceEpoch(predictionDateTime.toSecsSinceEpoch() + (60 * 60 * 24));
                    }
                }
            }
        }
    }
    //Draw white box for dates
    theColor.setRgb(255,255,255);
    thePen.setColor(theColor);
    theBrush.setColor(theColor);
    theBrush.setStyle(Qt::SolidPattern);
    painter.setPen(thePen);
    painter.setBrush(theBrush);
    painter.drawRect(0,height - verticalBorder + 3,width,height);

    //Draw date lines
    theColor.setRgb(22,45,245);
    thePen.setColor(theColor);
    thePen.setWidth(1);
    thePen.setStyle(Qt::SolidLine);
    secondPen.setStyle(Qt::DotLine);
    secondPen.setWidth(3);
    theColor.setRgb(22,45,245,50);
    secondPen.setColor(theColor);
    painter.setPen(thePen);
    unsigned int numberOfDateLabels = ceil((float)(width - horizontalBorder) / (float)labelHeight);
    qint64 secondsBetweenStartAndEnd = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
    unsigned int numberOfDaysBetweenStartAndEnd = floor((float)secondsBetweenStartAndEnd / 86400);
    unsigned int dayDivisor = ceil((float)numberOfDaysBetweenStartAndEnd / (float)numberOfDateLabels);
    unsigned int totalNumberOfDateLabels = floor((float)numberOfDaysBetweenStartAndEnd / (float)dayDivisor);
    QDateTime currentDate;
    int dateX;

    i = 0;
    while((unsigned int)i < totalNumberOfDateLabels)
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
    //Draw white box for weights
    theColor.setRgb(255,255,255);
    thePen.setColor(theColor);
    theBrush.setColor(theColor);
    theBrush.setStyle(Qt::SolidPattern);
    painter.setPen(thePen);
    painter.setBrush(theBrush);
    painter.drawRect(0,0,horizontalBorder - 5,height);


    //Draw weight lines
    theColor.setRgb(249,117,126);
    thePen.setColor(theColor);
    thePen.setWidth(1);
    thePen.setStyle(Qt::SolidLine);
    secondPen.setStyle(Qt::DotLine);
    secondPen.setWidth(3);
    theColor.setRgb(249,117,126,50);
    secondPen.setColor(theColor);
    painter.setPen(thePen);
    unsigned int numberOfWeightLabels = floor((float)(height - verticalBorder) / (float)labelHeight);
    unsigned int numberOfTenPoundsBetweenHighAndLowWeights = ceil((highWeight - lowWeight) / 10);
    unsigned int weightDivisor = ceil((float)numberOfTenPoundsBetweenHighAndLowWeights / (float)numberOfWeightLabels);
    unsigned int totalNumberOfWeightLabels = floor((float)numberOfTenPoundsBetweenHighAndLowWeights / (float)weightDivisor);
    float currentWeight;
    int weightLocationY;

    i = 0;
    while((unsigned int)i < totalNumberOfWeightLabels)
    {
        currentWeight = floor(highWeight / 10) * 10;
        currentWeight = currentWeight - (10 * (weightDivisor * i));
        weightPercent = (currentWeight - lowWeight) / (highWeight - lowWeight);
        if(weightPercent >= 0)
        {
            weightLocationY = (height - verticalBorder) - ((height - verticalBorder) * weightPercent);
            painter.setPen(secondPen);
            painter.drawLine(horizontalBorder, weightLocationY, width, weightLocationY);
            painter.setPen(thePen);
            painter.drawLine(horizontalBorder, weightLocationY, width, weightLocationY);
            painter.drawText(horizontalBorder - 20,weightLocationY + 5,QString::number(currentWeight));
        }
        i++;
    }

    //Draw Target Line
    if(showTargetWeight)
    {
        theColor.setRgb(113,255,45);
        thePen.setColor(theColor);
        thePen.setStyle(Qt::DotLine);
        thePen.setWidth(3);
        painter.setPen(thePen);
        float targetWeight = theDataAnalysis->getTargetWeight();
        weightPercent = (targetWeight - lowWeight) / (highWeight - lowWeight);
        if(weightPercent >=0)
        {
            weightLocationY = (height - verticalBorder) - ((height - verticalBorder) * weightPercent);
            painter.drawLine(horizontalBorder, weightLocationY, width, weightLocationY);
        }

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

void Graph::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {

        mouseDown = true;
        previousMouseDownX = ev->x();
        previousMouseDownY = ev->y();
    }
}

void Graph::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        mouseDown = false;
    }
}

void Graph::mouseMoveEvent(QMouseEvent *ev)
{
    if(mouseDown)
    {
        float differenceY = (highWeight - lowWeight) / (this->height() - verticalBorder);
        float differenceX = (endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch()) / (this->width() - horizontalBorder);
        int movedY = ev->y() - previousMouseDownY;
        int movedX = ev->x() - previousMouseDownX;
        previousMouseDownY = ev->y();
        previousMouseDownX = ev->x();
        lowWeight += movedY * differenceY;
        highWeight += movedY * differenceY;
        endDate = QDateTime::fromSecsSinceEpoch(endDate.toSecsSinceEpoch() - (movedX * differenceX));
        startDate = QDateTime::fromSecsSinceEpoch(startDate.toSecsSinceEpoch() - (movedX * differenceX));
        update();
    }
}

void Graph::wheelEvent(QWheelEvent *ev)
{
    float zoomFactor = 0.1;
    float angle = ev->angleDelta().ry();
    float screenRatio = (float)((float)this->width() - (float)horizontalBorder) / (float)((float)this->height() - (float)verticalBorder);
    float differenceX = (endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch()) / (float)((float)this->width() - (float)horizontalBorder);
    float differenceY = (float)(highWeight - lowWeight) / (float)((float)this->height() - (float)verticalBorder);
    float centerX = ((float)this->width() - (float)horizontalBorder) / 2;
    float centerY = ((float)this->height() - (float)verticalBorder) / 2;
    float movedX = (((float)ev->x() - (float)horizontalBorder) - centerX) * differenceX;
    float movedY = (centerY - (float)ev->y()) * differenceY;
    endDate = QDateTime::fromSecsSinceEpoch(endDate.toSecsSinceEpoch() + movedX);
    startDate = QDateTime::fromSecsSinceEpoch(startDate.toSecsSinceEpoch() + movedX);
    lowWeight += movedY;
    highWeight += movedY;
    float dataDifference = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
    float cropedData = dataDifference * screenRatio;
    endDate = QDateTime::fromSecsSinceEpoch(endDate.toSecsSinceEpoch() - (dataDifference / 2) + (cropedData / 2));
    startDate = QDateTime::fromSecsSinceEpoch(startDate.toSecsSinceEpoch() + (dataDifference / 2) - (cropedData / 2));
    if(angle < 0)
    {
        zoomFactor = fabs(zoomFactor);
    }
    else
    {
        zoomFactor = -fabs(zoomFactor);
    }
    dataDifference = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
    cropedData = dataDifference * (1 + zoomFactor);
    endDate = QDateTime::fromSecsSinceEpoch(endDate.toSecsSinceEpoch() - (dataDifference / 2) + (cropedData / 2));
    startDate = QDateTime::fromSecsSinceEpoch(startDate.toSecsSinceEpoch() + (dataDifference / 2) - (cropedData / 2));

    dataDifference = highWeight - lowWeight;
    cropedData = dataDifference * (1 + zoomFactor);
    highWeight = highWeight - (dataDifference / 2) + (cropedData / 2);
    lowWeight = lowWeight + (dataDifference / 2) - (cropedData / 2);

    dataDifference = endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch();
    cropedData = dataDifference * (1/screenRatio);
    endDate = QDateTime::fromSecsSinceEpoch(endDate.toSecsSinceEpoch() - (dataDifference / 2) + (cropedData / 2));
    startDate = QDateTime::fromSecsSinceEpoch(startDate.toSecsSinceEpoch() + (dataDifference / 2) - (cropedData / 2));

    differenceX = (float)(endDate.toSecsSinceEpoch() - startDate.toSecsSinceEpoch()) / ((float)this->width() - (float)horizontalBorder);
    differenceY = (highWeight - lowWeight) / ((float)this->height() - (float)verticalBorder);
    movedX = (centerX - ((float)ev->x() - (float)horizontalBorder)) * differenceX;
    movedY = ((float)ev->y() - centerY) * differenceY;
    endDate = QDateTime::fromSecsSinceEpoch(endDate.toSecsSinceEpoch() + movedX);
    startDate = QDateTime::fromSecsSinceEpoch(startDate.toSecsSinceEpoch() + movedX);
    lowWeight += movedY;
    highWeight += movedY;
    update();
}
