#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include "profile.h"
#include "entry.h"
#include "bmrdata.h"


class DataAnalysis
{
public:
    DataAnalysis();
    DataAnalysis(Profile* inProfile, std::vector<Entry>* inListOfEntries);
    ~DataAnalysis();
    void setProfile(Profile* inProfile);
    void setListOfEntries(std::vector<Entry>* inListOfEntries);
    void updateVariables();
    float getWeightRange()const;
    float getMinWeight()const;
    float getMaxWeight()const;
    int getUnaccountedForCalories()const;
    unsigned int getSizeOfBMRData() const;
    float getLowerBMRWeightAt(unsigned int inIndex) const;
    float getUpperBMRWeightAt(unsigned int inIndex) const;
    qint64 getBMRDataSecondsSinceEpoch(unsigned int inIndex) const;
    QDate getDateOfGoal() const;
    float getPredictedWeight(const QDateTime& inDateTime, float inPreviousWeight, int inCalories) const;
    float getEstimatedFatBurned()const;
    int getAverageCaloriesPerDay() const;
    float getTargetWeight() const;
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
