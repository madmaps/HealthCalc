#ifndef GRAPH_H
#define GRAPH_H
#include <QLabel>
#include <QDate>
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
    void paintEvent(QPaintEvent*);

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
};

#endif // GRAPH_H
