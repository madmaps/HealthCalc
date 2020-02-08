#include "dataanalysis.h"

DataAnalysis::DataAnalysis()
{
    theProfile = 0;
    listOfEntries = 0;
    bmrData = 0;
    minWeight = 0;
    maxWeight = 0;
    unaccountedForCalories = 0;
    weightRange = 0;
    averageCaloriesPerDay = 0;
}

DataAnalysis::DataAnalysis(Profile* inProfile, std::vector<Entry>* inListOfEntries)
{
    theProfile = inProfile;
    listOfEntries = inListOfEntries;
    bmrData = 0;
    minWeight = 0;
    maxWeight = 0;
    unaccountedForCalories = 0;
    weightRange = 0;
    averageCaloriesPerDay = 0;
}

DataAnalysis::~DataAnalysis()
{
    if(bmrData != 0)
    {
        bmrData->clear();
        delete bmrData;
        bmrData = 0;
    }
}


void DataAnalysis::setProfile(Profile* inProfile)
{
    theProfile = inProfile;
}

void DataAnalysis::setListOfEntries(std::vector<Entry>* inListOfEntries)
{
    listOfEntries = inListOfEntries;
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

int DataAnalysis::getAverageCaloriesPerDay() const
{
    return averageCaloriesPerDay;
}

unsigned int DataAnalysis::getSizeOfBMRData() const
{
    if(bmrData !=0)
    {
        return bmrData->size();
    }
    return 0;
}

float DataAnalysis::getLowerBMRWeightAt(unsigned int inIndex) const
{
    if(bmrData != 0)
    {
        return bmrData->at(inIndex).getOutputWeight();
    }
    return 0;
}

float DataAnalysis::getUpperBMRWeightAt(unsigned int inIndex) const
{
    if(bmrData != 0)
    {
        return bmrData->at(inIndex).getOutputWeight() + weightRange;
    }
    return 0;
}

qint64 DataAnalysis::getBMRDataSecondsSinceEpoch(unsigned int inIndex) const
{
    if(bmrData != 0)
    {
        return bmrData->at(inIndex).getDateTime().toSecsSinceEpoch();
    }
    return 0;
}

QDate DataAnalysis::getDateOfGoal() const
{
    QDateTime returnDate;
    bool found = false;
    QDateTime predictionDate = listOfEntries->at(listOfEntries->size()-1).getDateTime();
    float predictionWeight = bmrData->at(bmrData->size()-1).getOutputWeight();
    for(unsigned int i = 0; i <= (365 * 3) && !found; i++)
    {
        predictionDate = QDateTime::fromSecsSinceEpoch(predictionDate.toSecsSinceEpoch() + (60 * 60 * 24));
        predictionWeight = getPredictedWeight(predictionDate,predictionWeight,averageCaloriesPerDay);
        if(predictionWeight + weightRange < theProfile->getTargetWeight() && !found)
        {
            found = true;
            returnDate = predictionDate;
        }
    }
    return returnDate.date();
}

float DataAnalysis::getEstimatedFatBurned()const
{
    return bmrData->at(0).getOutputWeight() - bmrData->at(bmrData->size()-1).getOutputWeight();
}


float DataAnalysis::getPredictedWeight(const QDateTime &inDateTime, float inPreviousWeight, int inCalories) const
{
    float BMR = 0;
    int age = (float)theProfile->getDob().daysTo(inDateTime.date()) / 365;
    if(theProfile->getGender())
    {
        BMR = 66 + (6.3 * inPreviousWeight) + (12.9 * (float)theProfile->getHeight()) - (6.8 * (float)age);
    }
    else
    {
        BMR = 655 + (4.3 * inPreviousWeight) + (4.7 * (float)theProfile->getHeight()) - (4.7 * (float)age);
    }
    BMR *= 1.2;
    float outputWeight = inPreviousWeight - (float)(BMR - inCalories) / 3500;
    return outputWeight;
}

void DataAnalysis::updateMaxWeight()
{
    if(listOfEntries !=0)
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
}

void DataAnalysis::updateMinWeight()
{
    if(listOfEntries != 0)
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
}

void DataAnalysis::updateUnaccountedForCalories()
{
    if(listOfEntries !=0)
    {
        const int tryCalorieRange = 1000;
        float calorieOffset = 0;
        float lowestDifferenceBetweenHighAndLow = 9999;
        float tryOffset;
        for(int k = -tryCalorieRange;k <= tryCalorieRange;k+=5)
        {
            if(k >= tryCalorieRange)
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
                if(bmrData == 0)
                {
                    bmrData = new std::vector<BMRData>;
                }
                bmrData->clear();
                bmrData->push_back(BMRData(theProfile,&listOfEntries->at(0),0,theProfile->getInitialWeight(),tryOffset));
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
}

void DataAnalysis::updateAverageCaloriesPerDay()
{
    if(listOfEntries != 0)
    {
        unsigned int i = 0;
        float totalCalories = 0;
        while(i < listOfEntries->size())
        {
            totalCalories += (int)listOfEntries->at(i).getCaloriesConsumed() - (int)listOfEntries->at(i).getCaloriesBurned();
            i++;
        }
        float totalDays = (float)(listOfEntries->at(listOfEntries->size()-1).getDateTime().toSecsSinceEpoch() - listOfEntries->at(0).getDateTime().toSecsSinceEpoch())/ (60 * 60 * 24);
        averageCaloriesPerDay = totalCalories / totalDays;
        averageCaloriesPerDay += unaccountedForCalories;
    }
}
