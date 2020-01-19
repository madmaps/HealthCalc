#ifndef GRAPH_H
#define GRAPH_H
#include <QLabel>
#include <QDate>
#include "entry.h"



class Graph : public QLabel
{   
    Q_OBJECT
public:
    explicit Graph(QWidget* parent = 0);

    void setListOfEntries(std::vector<Entry>* inListOfEntries);

    void updateVariables();

protected:
    void paintEvent(QPaintEvent*);

private:
    std::vector<Entry>* listOfEntries;
    QDate startDate;
    QDate endDate;
    bool autoDate;
    unsigned int lowWeight;
    unsigned int highWeight;
    bool autoWeight;
    float minWeight, maxWeight;
};

#endif // GRAPH_H
