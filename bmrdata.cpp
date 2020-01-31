#include "bmrdata.h"

BMRData::BMRData(Profile* inProfile,Entry* inEntry,Entry* inPreviousEntry,float inPreviousWeight,int inOtherCalories)
{
    otherCalories = inOtherCalories;
    theProfile = inProfile;
    theEntry = inEntry;
    thePreviousEntry = inPreviousEntry;
    initialWeight = inPreviousWeight;
    calculateBMR();
    calculateOutputWeight();
}

float BMRData::getOutputWeight()const
{
    return outputWeight;
}

QDateTime BMRData::getDateTime() const
{
    return theEntry->getDateTime();
}

void BMRData::shift(float inAmount)
{
    outputWeight += inAmount;
}


void BMRData::calculateBMR()
{
    unsigned int age = (float)theProfile->getDob().daysTo(theEntry->getDateTime().date()) / 365;
    if(theProfile->getGender())
    {
        BMR = 66 + (6.3 * initialWeight) + (12.9 * theProfile->getHeight()) - (6.8 * age);
    }
    else
    {
        BMR = 655 + (4.3 * initialWeight) + (4.7 * theProfile->getHeight()) - (4.7 * age);
    }
    BMR *= 1.2;
}

void BMRData::calculateOutputWeight()
{
    int caloriesPerDay;
    if(thePreviousEntry != 0)
    {

        caloriesPerDay = (float)(theEntry->getCaloriesConsumed() - theEntry->getCaloriesBurned()) / (float)((float)(theEntry->getDateTime().toSecsSinceEpoch() - thePreviousEntry->getDateTime().toSecsSinceEpoch()) / 86400);
        caloriesPerDay += otherCalories;
        outputWeight = initialWeight - ((float)(BMR - caloriesPerDay) / 3500) * ((float)(theEntry->getDateTime().toSecsSinceEpoch() - thePreviousEntry->getDateTime().toSecsSinceEpoch()) / 86400);

    }
    else
    {
        caloriesPerDay = theEntry->getCaloriesConsumed() - theEntry->getCaloriesBurned();
        caloriesPerDay += otherCalories;
        outputWeight = initialWeight - (float)(BMR - caloriesPerDay) / 3500;

    }

}
