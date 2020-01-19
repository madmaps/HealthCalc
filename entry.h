#ifndef ENTRY_H
#define ENTRY_H
#include <QDateTime>
#include <QString>
#include <vector>


class Entry
{
public:
    Entry(const float& inWeight,const QDateTime& inDateTime,const unsigned int& inCaloriesConsumed,const unsigned int& inCaloriesBurned);
    Entry(const Entry& inEntry);
    Entry(const std::vector<char>& inEntryData);

    float getWeight()const;

    Entry operator=(const Entry& inEntry);
    bool operator<(const Entry& inEntry);
    QString toString();
    std::vector<char>* saveEntryData()const;
private:
    float weight;
    QDateTime dateTime;
    unsigned int caloriesConsumed;
    unsigned int caloriesBurned;

};

#endif // ENTRY_H
