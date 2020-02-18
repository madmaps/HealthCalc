#ifndef GRAPH_H
#define GRAPH_H
#include <QLabel>
#include <QDate>
#include <QMouseEvent>
#include <QWheelEvent>
#include "entry.h"
#include "bmrdata.h"
#include "dataanalysis.h"



class Graph : public QLabel
{   
    Q_OBJECT
public:
    explicit Graph(QWidget* parent = 0);

    void setListOfEntries(std::vector<Entry>* inListOfEntries);
    void setDataAnalysis(DataAnalysis* inDataAnalysis);
    void updateVariables();
    void setAutoDate(const bool& inAutoDate);
    void setAutoWeight(const bool& inAutoWeight);
    void setStartDate(const QDate& inDate);
    void setEndDate(const QDate& inDate);
    void setlowWeight(const float& inLowWeight);
    void setHighWeight(const float& inHighWeight);
    void setShowPrediction(const bool& inShowPrediction);
    void setShowWeightRange(const bool& inShowWeightRange);
    void setShowTargetWeight(const bool& inShowTargetWeight);


protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *ev) override;

    void wheelEvent(QWheelEvent *ev) override;

private:
    std::vector<Entry>* listOfEntries;
    DataAnalysis* theDataAnalysis;
    QDateTime startDate;
    QDateTime endDate;
    bool autoDate;
    float lowWeight;
    float highWeight;
    bool autoWeight;
    float minWeight, maxWeight;
    bool showPrediction;
    bool showWeightRange;
    bool showTargetWeight;
    bool mouseDown;
    int previousMouseDownX;
    int previousMouseDownY;
    int horizontalBorder;
    int verticalBorder;
};

#endif // GRAPH_H
