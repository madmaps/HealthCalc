#include "entry.h"

Entry::Entry(const float& inWeight,const QDateTime& inDateTime,const unsigned int& inCaloriesConsumed,const unsigned int& inCaloriesBurned)
{
    weight = inWeight;
    dateTime = inDateTime;
    caloriesConsumed = inCaloriesConsumed;
    caloriesBurned = inCaloriesBurned;

}

Entry::Entry(const Entry& inEntry) : weight(inEntry.weight), dateTime(inEntry.dateTime), caloriesConsumed(inEntry.caloriesConsumed), caloriesBurned(inEntry.caloriesBurned)
{
}

float Entry::getWeight()const
{
    return weight;
}

QDateTime Entry::getDateTime()const
{
    return dateTime;
}

Entry::Entry(const std::vector<char>& inEntryData)
{
    unsigned int dataPlace = 0;
    qint64 dateTimeData = 0;
    for(unsigned int i = 0; i <= 7; i++)
    {
        dateTimeData |= (unsigned char)inEntryData.at(dataPlace) << (i * 8);
        dataPlace++;
    }
    dateTime = QDateTime::fromSecsSinceEpoch(dateTimeData);

    unsigned int weightData = 0;
    for(unsigned int i = 0; i <= 3; i++)
    {
        weightData |= (unsigned char)inEntryData.at(dataPlace) << (i * 8);
        dataPlace++;
    }
    weight = (float)weightData/1000;

    caloriesConsumed = 0;
    for(unsigned int i = 0; i <= 3; i++)
    {
        caloriesConsumed |= (unsigned char)inEntryData.at(dataPlace) << (i * 8);
        dataPlace++;
    }

    caloriesBurned = 0;
    for(unsigned int i = 0; i <= 3; i++)
    {
        caloriesBurned |= (unsigned char)inEntryData.at(dataPlace) << (i * 8);
        dataPlace++;
    }
}


Entry Entry::operator=(const Entry& inEntry)
{
    if(this != &inEntry)
    {
        weight = inEntry.weight;
        dateTime = inEntry.dateTime;
        caloriesConsumed = inEntry.caloriesConsumed;
        caloriesBurned = inEntry.caloriesBurned;
    }
    return *this;
}

bool Entry::operator<(const Entry &inEntry)
{
    return dateTime.msecsTo(inEntry.dateTime)>0;
}

QString Entry::toString()
{
    QString returnString;
    returnString = dateTime.toString("MM/dd/yy");
    returnString += "  ";
    returnString += QString::number(weight);
    returnString += "  ";
    returnString += QString::number(caloriesConsumed);
    returnString += "  ";
    returnString += QString::number(caloriesBurned);
    return returnString;
}

std::vector<char>* Entry::saveEntryData()const
{
    std::vector<char>* fileData = new std::vector<char>;
    char* returnData = new char[16];

    qint64 returnDateTime = dateTime.toSecsSinceEpoch();
    for(int i = 0; i <= 7; i++)
    {
        returnData[0] = (returnDateTime >> (i*8)) & 0xff;
        fileData->push_back(returnData[0]);
    }

    unsigned int returnWeight = weight * 1000;
    for(int i = 0; i <= 3; i++)
    {
        returnData[0] = (returnWeight >> (i*8)) & 0xff;
        fileData->push_back(returnData[0]);
    }

    for(int i = 0; i <= 3; i++)
    {
        returnData[0] = (caloriesConsumed >> (i*8)) & 0xff;
        fileData->push_back(returnData[0]);
    }

    for(int i = 0; i <= 3; i++)
    {
        returnData[0] = (caloriesBurned >> (i*8)) & 0xff;
        fileData->push_back(returnData[0]);
    }

    delete[] returnData;
    return fileData;
}

