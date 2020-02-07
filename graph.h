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
    int getUnaccountedCalories()const;
    float getWeightRange()const;

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
};

#endif // GRAPH_H
