#ifndef PROFILE_H
#define PROFILE_H
#include <QString>
#include <QDate>
#include <QFile>
#include <QByteArray>


class Profile
{
public:
    Profile(const QString& inName,const QDate& inDob,unsigned int inHeight, bool inGender,float inInitialWeight,float inTargetWeight);
    Profile(const Profile& inProfile);
    Profile(const QByteArray& inProfileData);

    QString getName() const;
    QDate getDob() const;
    unsigned int getHeight() const;
    bool getGender()const;
    float getInitialWeight() const;
    float getTargetWeight() const;
    void setTargetWeight(const float& inTargetWeight);

    std::vector<char>* saveToFile() const;

private:
    QString name;
    QDate dob;
    unsigned int height;
    bool gender;
    float initialWeight;
    float targetWeight;


};

#endif // PROFILE_H
