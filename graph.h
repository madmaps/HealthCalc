#ifndef GRAPH_H
#define GRAPH_H
#include <QLabel>
#include <QDate>



class Graph : public QLabel
{   
    Q_OBJECT
public:
    explicit Graph(QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent*);

private:
    QDate startDate;
    QDate endDate;
    bool autoDate;
    unsigned int lowWeight;
    unsigned int highWeight;
    bool autoWeight;
};

#endif // GRAPH_H
