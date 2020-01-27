#ifndef BMRDATA_H
#define BMRDATA_H
#include "profile.h"
#include "entry.h"


class BMRData
{
public:
    BMRData(Profile* inProfile,Entry* inEntry,Entry* inPreviousEntry,float inPreviousWeight);

    unsigned int getOutputWeight()const;
private:
    void calculateBMR();
    void calculateOutputWeight();

    unsigned int BMR;
    float initialWeight;
    float outputWeight;
    Profile* theProfile;
    Entry* theEntry;
    Entry* thePreviousEntry;
};

#endif // BMRDATA_H
