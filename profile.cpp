#include "profile.h"

Profile::Profile(const QString& inName,const QDate& inDob,unsigned int inHeight, bool inGender,float inInitialWeight,float inTargetWeight)
{
    name = inName;
    dob = inDob;
    height = inHeight;
    gender = inGender;
    initialWeight = inInitialWeight;
    targetWeight = inTargetWeight;
}

Profile::Profile(const Profile& inProfile) : name(inProfile.name), dob(inProfile.dob), height(inProfile.height), gender(inProfile.gender), initialWeight(inProfile.initialWeight), targetWeight(inProfile.targetWeight)
{

}

Profile::Profile(const QByteArray& inProfileData)
{
    const int header = 9;
    unsigned char highByte,lowByte;
    int numberOfChunks = inProfileData.at(8);
    int dataSize;
    char chunkID;
    int dataLocation = 0;
    int j, nameSize;
    unsigned int localDataLocation = 0;
    int weightData = 0;
    qint64 dateData;

    int i = 0;
    while (i<numberOfChunks)
    {
        chunkID = inProfileData.at(dataLocation + header);
        switch(chunkID)
        {
        case 0x05:
            localDataLocation = 3;
            nameSize = inProfileData.at(header + dataLocation + localDataLocation);
            localDataLocation++;
            j = 0;
            while(j < nameSize)
            {
                name += inProfileData.at(dataLocation + header + localDataLocation);
                j++;
                localDataLocation++;
            }

            dateData = 0;
            for(unsigned int k = 0;k <= 7; k++)
            {
                dateData |= inProfileData.at(header + dataLocation + localDataLocation) << (8 * k);
                localDataLocation++;
            }
            dob = QDate::fromJulianDay(dateData);

            lowByte = inProfileData.at(dataLocation + header + localDataLocation);
            height = ((lowByte >> 1) & 0x7f);

            if((lowByte & 0x01) == 0x01)
            {
                gender = true;
            }
            else
            {
                gender = false;
            }
            localDataLocation++;

            weightData = 0;
            for(unsigned int k = 0; k <= 3; k++)
            {
                weightData |= (unsigned char)inProfileData.at(dataLocation + header + localDataLocation) << (k * 8);
                localDataLocation++;
            }
            initialWeight = (float)weightData/1000;

            weightData = 0;
            for(unsigned int k = 0; k <= 3; k++)
            {
                weightData |= (unsigned char)inProfileData.at(dataLocation + header + localDataLocation) << (k * 8);
                localDataLocation++;
            }
            targetWeight = (float)weightData/1000;

            dataSize = 0;
            lowByte = inProfileData.at(dataLocation + header + 1);
            highByte = inProfileData.at(dataLocation + header + 2);
            dataSize = ((unsigned char)highByte << 8) | (unsigned char)lowByte;
            dataLocation += dataSize;
            break;

        default:
            dataSize = 0;
            lowByte = inProfileData.at(dataLocation + header + 1);
            highByte = inProfileData.at(dataLocation + header + 2);
            dataSize = ((unsigned char)highByte << 8) | (unsigned char)lowByte;
            dataLocation += dataSize;
            break;
        }
        i++;
    }
}



QString Profile::getName()const
{
    return name;
}

QDate Profile::getDob()const
{
    return dob;
}

unsigned int Profile::getHeight()const
{
    return height;
}

bool Profile::getGender()const
{
    return gender;
}

float Profile::getInitialWeight()const
{
    return initialWeight;
}

float Profile::getTargetWeight()const
{
    return targetWeight;
}

std::vector<char>* Profile::saveToFile()const
{
    char* returnData = new char[16];
    std::vector<char>* fileData = new std::vector<char>;

    // ChunkID ***Profile Data's ID is 0x05***
    returnData[0] = 0x05;
    fileData->push_back(returnData[0]);

    // Overall Size - Header(3 bytes) + nameLength(1byte) + nameData(?bytes) + dob(8 bytes) + Height&Gender(7bits&1bit) + initialWeight(4bytes) + targetWeight(4bytes)
    int returnValue = 3 + 1 + name.size() + 8 + 1 + 4 + 4;
    returnData[0] = (unsigned char)(returnValue & 0xff);
    returnData[1] = (unsigned char)((returnValue >> 8) & 0xff);
    fileData->push_back(returnData[0]);
    fileData->push_back(returnData[1]);

    // Length of name
    returnValue = name.size();
    returnData[0] = (unsigned char)(returnValue & 0xff);
    fileData->push_back(returnData[0]);

    // Name data
    int i = 0;
    while(i < name.size())
    {
        fileData->push_back(name.toLocal8Bit().at(i));
        i++;
    }

    // Date of Birth Data (BITS  mmmmdddd dyyyyyyy) - year range from 1924 to 2052
    qint64 returnDate = dob.toJulianDay();
    for(int i = 0; i <= 7; i++)
    {
        returnData[0] = (returnDate >> (i*8)) & 0xff;
        fileData->push_back(returnData[0]);
    }

    // Height Data & Gender (Bits hhhhhhhg)
    returnValue = ((height & 0x7f) << 1);
    if(gender)
    {
        returnValue |= 0x01;
    }
    returnData[0] = (unsigned char)(returnValue & 0xff);
    fileData->push_back(returnData[0]);

    // Initial Weight Data(4 bytes) Data * 10
    returnValue = (initialWeight * 1000);
    for(int i = 0; i <= 3; i++)
    {
        returnData[0] = (returnValue >> (i*8)) & 0xff;
        fileData->push_back(returnData[0]);
    }

    // Target Weight Data(4 bytes) Data * 10
    returnValue = (targetWeight * 1000);
    for(int i = 0; i <= 3; i++)
    {
        returnData[0] = (returnValue >> (i*8)) & 0xff;
        fileData->push_back(returnData[0]);
    }
    return fileData;
}
