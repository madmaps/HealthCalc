#ifndef GRAPH_H
#define GRAPH_H
#include <QLabel>
#include <QDate>
#include "entry.h"
#include "bmrdata.h"
#include "profile.h"



class Graph : public QLabel
{   
    Q_OBJECT
public:
    explicit Graph(QWidget* parent = 0);

    void setListOfEntries(std::vector<Entry>* inListOfEntries);
    void setProfile(Profile* inProfile);

    void updateVariables();

protected:
    void paintEvent(QPaintEvent*);

private:
    std::vector<Entry>* listOfEntries;
    Profile* theProfile;
    std::vector<BMRData>* lowerBmrData;
    std::vector<BMRData>* upperBmrData;
    QDateTime startDate;
    QDateTime endDate;
    bool autoDate;
    float lowWeight;
    float highWeight;
    bool autoWeight;
    float minWeight, maxWeight;
};

#endif // GRAPH_H
