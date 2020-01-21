#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    listOfEntries = new std::vector<Entry>();
    profileLoaded = false;
    ui->setupUi(this);
    ui->label->setListOfEntries(listOfEntries);
    disableAll(true);
    entryWindow = new AddEntry(this);
    entryWindow->hide();
    newProfile = new NewProfile(this);
    newProfile->hide();
   // profile = 0;
    profile = new Profile(QString("Matthew"),QDate(1983,9,12),67,1,235,170);
    updateProfile();


    connect(entryWindow,SIGNAL(addNewEntry(float,QDateTime,unsigned int, unsigned int)),this,SLOT(gettingNewEntry(float,QDateTime,unsigned int,unsigned int)));
    connect(newProfile,SIGNAL(newProfile(Profile)),this,SLOT(gettingNewProfile(Profile)));
}

MainWindow::~MainWindow()
{
    delete listOfEntries;
    delete profile;
    delete newProfile;
    delete entryWindow;
    delete ui;
}

void MainWindow::gettingNewEntry(float inWeight,QDateTime inDateTime,unsigned int inCaloriesConsumed,unsigned int inCaloriesBurned)
{
    Entry newEntry(inWeight,inDateTime,inCaloriesConsumed,inCaloriesBurned);
    listOfEntries->push_back(newEntry);
    updateEntries();
}

void MainWindow::gettingNewProfile(Profile inProfile)
{
    if(profile != 0)
    {
        delete profile;
        profile = 0;
    }
    profile = new Profile(inProfile);
    updateProfile();

}

void MainWindow::on_newEntryButton_clicked()
{
    entryWindow->clearEntry();
    entryWindow->show();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_removeEntryButton_clicked()
{
    if(listOfEntries->size()>0)
    {
        unsigned int index = ui->listWidget->currentRow();
        unsigned long long i = index;
        while(i<listOfEntries->size()-1)
        {
            listOfEntries->at(i) = listOfEntries->at(i+1);
            i++;
        }
        listOfEntries->pop_back();
        updateEntries();
    }
}

void MainWindow::updateEntries()
{
    std::sort(listOfEntries->begin(),listOfEntries->end());
    ui->listWidget->clear();
    for(Entry display : *listOfEntries)
    {
        ui->listWidget->addItem(display.toString());
    }
    ui->listWidget->update();
    ui->label->updateVariables();
    ui->label->update();
}

void MainWindow::on_actionNew_Profile_triggered()
{
    disableAll(false);
    newProfile->show();
}

void MainWindow::disableAll(bool inDisable)
{
    ui->newEntryButton->setDisabled(inDisable);
    ui->removeEntryButton->setDisabled(inDisable);
    ui->startDateEditBox->setDisabled(inDisable);
    ui->endDateEditBox->setDisabled(inDisable);
    ui->highWeightComboBox->setDisabled(inDisable);
    ui->lowWeightComboBox->setDisabled(inDisable);
    ui->autoDateCheckBox->setDisabled(inDisable);
    ui->autoWeightCheckBox->setDisabled(inDisable);
    ui->targetWeightEditBox->setDisabled(inDisable);
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"));
    QFile outFile(fileName);
    if(outFile.open(QIODevice::WriteOnly))
    {
        char* temp = new char[16];
        //File Type **HEAC**
        temp[0] = 0x48;
        temp[1] = 0x45;
        temp[2] = 0x41;
        temp[3] = 0x43;
        outFile.write(temp,4);

        //File Version
        temp[0] = 0x00;
        temp[1] = 0x01;
        outFile.write(temp,2);

        //File Size **Not used**
        temp[0] = 0x00;
        temp[1] = 0x00;
        outFile.write(temp,2);

        //Number of Chunks 1-ProfileData, 2-WeightData, 3-ApplicationSettings
        temp[0] = 0x02;
        outFile.write(temp,1);

        std::vector<char>* profileData = profile->saveToFile();
        for(unsigned long long i = 0; i < profileData->size(); i++)
        {
            temp[0] = profileData->at(i);
            outFile.write(temp,1);
        }
        delete profileData;

        temp[0] = 0x06;
        outFile.write(temp,1);
        unsigned int entryDataSize = 3 + 20 * listOfEntries->size();
        temp[0] = (unsigned char)(entryDataSize & 0xff);
        temp[1] = (unsigned char)((entryDataSize >> 8) & 0xff);
        outFile.write(temp,2);

        unsigned int i = 0;
        while(i < listOfEntries->size())
        {
            std::vector<char>* entryData = listOfEntries->at(i).saveEntryData();
            for(unsigned int j = 0; j < entryData->size(); j++)
            {
                temp[0] = entryData->at(j);
                outFile.write(temp,1);
            }
            delete entryData;
            i++;
        }
    delete[] temp;
    }
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"));
    if(fileName!="")
    {
        QFile inFile(fileName);
        if(inFile.open(QIODevice::ReadOnly))
        {
            const int header = 9;
            QByteArray data = inFile.readAll();
            inFile.close();
            if(data.at(0) == 0x48 && data.at(1) == 0x45 && data.at(2) == 0x41 && data.at(3) == 0x43 && data.at(4) == 0x00 && data.at(5) == 0x01)
            {
                disableAll(false);
                unsigned int numberOfChunks = data.at(8);
                unsigned int chunkID = 0;
                unsigned int fileLocation = 0;
                char highByte, lowByte;
                unsigned int overallSize = 0;
                unsigned int numberOfEntries;
                std::vector<char>* entryData = new std::vector<char>;
                unsigned int j = 0;
                unsigned int i = 0;

                while(i < numberOfChunks)
                {
                    chunkID = data.at(header + fileLocation);
                    switch(chunkID)
                    {
                    case 0x05:
                        if(profile!=0)
                        {
                            delete profile;
                            profile = 0;
                        }
                        profile = new Profile(data);
                        updateProfile();

                        overallSize = 0;
                        lowByte = data.at(header + fileLocation + 1);
                        highByte = data.at(header + fileLocation + 2);
                        overallSize = ((unsigned char)highByte << 8) | (unsigned char)lowByte;
                        fileLocation += overallSize;
                        break;

                    case 0x06:
                        overallSize = 0;
                        lowByte = data.at(header + fileLocation + 1);
                        highByte = data.at(header + fileLocation + 2);
                        overallSize = ((unsigned char)highByte << 8) | (unsigned char)lowByte;
                        numberOfEntries = (float)overallSize / 20;
                        listOfEntries->clear();
                        j = 0;
                        while(j < numberOfEntries)
                        {
                            entryData->clear();
                            for(unsigned k = 0;k <= 19 ; k++)
                            {
                                entryData->push_back((unsigned char)data.at(header + fileLocation + 3 + (j * 20) + k));
                            }
                            listOfEntries->push_back(Entry(*entryData));
                            j++;
                        }
                        updateEntries();
                        ui->label->updateVariables();
                        entryData->clear();
                        delete entryData;
                        fileLocation += overallSize;
                        break;

                    default:
                        overallSize = 0;
                        lowByte = data.at(header + fileLocation + 1);
                        highByte = data.at(header + fileLocation + 2);
                        overallSize = ((unsigned char)highByte << 8) | (unsigned char)lowByte;
                        fileLocation += overallSize;
                        break;

                    }
                    i++;
                }
            }
        }
    }
}

void MainWindow::updateProfile()
{
    ui->nameEditBox->setText(profile->getName());
    ui->initialWeightEditBox->setText(QString::number(profile->getInitialWeight()));
    ui->dobEditBox->setDate(profile->getDob());
    QString theGender;
    if(profile->getGender())
    {
        theGender += "Male";
    }
    else
    {
        theGender += "Female";
    }
    ui->genderEditBox->setText(theGender);
    ui->heightEditBox->setText(QString::number(profile->getHeight()));
    ui->targetWeightEditBox->setText(QString::number(profile->getTargetWeight()));
}

void MainWindow::on_actionSave_Profile_triggered()
{
    saveFile();
}

void MainWindow::on_actionOpen_Projile_triggered()
{
    loadFile();
}
