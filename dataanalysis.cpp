#include "dataanalysis.h"

DataAnalysis::DataAnalysis()
{

}

void DataAnalysis::setProfile(Profile* inProfile)
{
    theProfile = inProfile;
}

void DataAnalysis::setListOfEntries(std::vector<Entry>* inListOfEntries)
{
    listOfEntries = inListOfEntries;
}

std::vector<Entry>* DataAnalysis::getListOfEntries()const
{
    return listOfEntries;
}

Profile* DataAnalysis::getProfile()const
{
    return theProfile;
}

void DataAnalysis::updateVariables()
{
    updateMinWeight();
    updateMaxWeight();
    updateUnaccountedForCalories();
    updateAverageCaloriesPerDay();
}

float DataAnalysis::getWeightRange()const
{
    return weightRange;
}

float DataAnalysis::getMinWeight()const
{
    return minWeight;
}

float DataAnalysis::getMaxWeight()const
{
    return maxWeight;
}

int DataAnalysis::getUnaccountedForCalories()const
{
    return unaccountedForCalories;
}

void DataAnalysis::updateMaxWeight()
{
    maxWeight = 0;
    for(Entry check : *listOfEntries)
    {
        if(check.getWeight()>maxWeight)
        {
            maxWeight = check.getWeight();
        }
    }
}

void DataAnalysis::updateMinWeight()
{
    minWeight = 999999;
    for(Entry check : *listOfEntries)
    {
        if(check.getWeight() < minWeight)
        {
            minWeight = check.getWeight();
        }
    }

}

void DataAnalysis::updateUnaccountedForCalories()
{
    const int tryCalorieRange = 1000;
    float calorieOffset = 0;
    float lowestDifferenceBetweenHighAndLow = 9999;
    float tryOffset;
    for(int k = -tryCalorieRange;k <= tryCalorieRange;k+=5)
    {
        if(k >= tryCalorieRange - 5)
        {
            tryOffset = calorieOffset;
            unaccountedForCalories = calorieOffset;
            weightRange = lowestDifferenceBetweenHighAndLow;
        }
        else
        {
            tryOffset = k;
        }
        if(listOfEntries->size() > 0)
        {
            if(bmrData != 0)
            {
                bmrData->clear();
                delete bmrData;
                bmrData = 0;
            }
            bmrData = new std::vector<BMRData>;
            Entry* testEntry = new Entry(listOfEntries->at(0));
            BMRData firstIn(theProfile,testEntry,0,theProfile->getInitialWeight(),tryOffset);
            bmrData->push_back(firstIn);
            unsigned int i = 1;
            while (i < listOfEntries->size())
            {
                bmrData->push_back(BMRData(theProfile,&listOfEntries->at(i),&listOfEntries->at(i-1),bmrData->at(i-1).getOutputWeight(),tryOffset));
                i++;
            }

            float biggestNegativeDifference = 0;
            i = 0;
            while(i < listOfEntries->size())
            {
                if(listOfEntries->at(i).getWeight() - bmrData->at(i).getOutputWeight() < biggestNegativeDifference)
                {
                    biggestNegativeDifference = listOfEntries->at(i).getWeight() - bmrData->at(i).getOutputWeight();
                }
                i++;
            }

            i = 0;
            while(i < bmrData->size())
            {
                bmrData->at(i).shift(biggestNegativeDifference);
                   i++;
            }

            float biggestPositiveDifference = 0;
            i = 0;
            while(i < listOfEntries->size())
            {
                if(listOfEntries->at(i).getWeight() - bmrData->at(i).getOutputWeight() > biggestPositiveDifference)
                {
                    biggestPositiveDifference = listOfEntries->at(i).getWeight() - bmrData->at(i).getOutputWeight();
                }
                i++;
            }
            if(biggestPositiveDifference < lowestDifferenceBetweenHighAndLow)
            {
                lowestDifferenceBetweenHighAndLow = biggestPositiveDifference;
                calorieOffset = k;
            }
        }
    }

}

void DataAnalysis::updateAverageCaloriesPerDay()
{
    unsigned int i = 0;
    float totalCalories = 0;
    while(i < listOfEntries->size())
    {
        totalCalories += listOfEntries->at(i).getCaloriesConsumed() - listOfEntries->at(i).getCaloriesBurned();
        i++;
    }
    float totalDays = (float)(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - listOfEntries->at(0).getDateTime().toSecsSinceEpoch())/ (60 * 60 * 24);
    averageCaloriesPerDay = totalCalories / totalDays;
    averageCaloriesPerDay += unaccountedForCalories;
}
