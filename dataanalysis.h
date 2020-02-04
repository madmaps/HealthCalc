#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include "profile.h"
#include "entry.h"
#include "bmrdata.h"


class DataAnalysis
{
public:
    DataAnalysis();
    void setProfile(Profile* inProfile);
    void setListOfEntries(std::vector<Entry>* inListOfEntries);
    std::vector<Entry>* getListOfEntries()const;
    Profile* getProfile()const;
    void updateVariables();
    float getWeightRange()const;
    float getMinWeight()const;
    float getMaxWeight()const;
    int getUnaccountedForCalories()const;

private:
    void updateMaxWeight();
    void updateMinWeight();
    void updateUnaccountedForCalories();
    void updateAverageCaloriesPerDay();


    Profile* theProfile;
    std::vector<Entry>* listOfEntries;
    std::vector<BMRData>* bmrData;
    float minWeight;
    float maxWeight;
    int unaccountedForCalories;
    float weightRange;
    int averageCaloriesPerDay;
};

#endif // DATAANALYSIS_H
