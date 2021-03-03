#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    listOfEntries = new std::vector<Entry>();
    profileLoaded = false;
    dataChanged = false;
    saveAs = true;
    closingProgram = false;
    wantNewProfile = false;
    loadProfile = false;
    closeProfile = false;
    ui->setupUi(this);
    ui->label->setListOfEntries(listOfEntries);
    currentFile = QString("");
    disableAll(true);
    entryWindow = new AddEntry(this);
    entryWindow->hide();
    newProfile = new NewProfile(this);
    newProfile->hide();
    newSavedData = new SaveData(this);
    newSavedData->hide();
    profile = new Profile(QString(""),QDate(1970,1,01),60,1,100,100);
    theDataAnalysis = new DataAnalysis(profile,listOfEntries);
    ui->weightRangeCheckbox->setCheckState(Qt::Checked);
    ui->label->setShowPrediction(true);
    ui->predictionCheckbox->setCheckState(Qt::Checked);
    ui->label->setShowWeightRange(true);
    ui->targetWeightCheckBox->setCheckState(Qt::Checked);
    ui->label->setShowTargetWeight(true);
    ui->label->setDataAnalysis(theDataAnalysis);
    ui->predefinedAutosComboBox->addItem(QString("All Data"));
    ui->predefinedAutosComboBox->addItem(QString("Past Week"));
    ui->predefinedAutosComboBox->addItem(QString("Past 2 Weeks"));
    ui->predefinedAutosComboBox->addItem(QString("Past 3 Weeks"));
    ui->predefinedAutosComboBox->addItem(QString("Past Month"));
    ui->predefinedAutosComboBox->addItem(QString("Full View"));
    ui->predefinedAutosComboBox->addItem(QString("Custom"));
    ui->predefinedAutosComboBox->setCurrentIndex(0);
    ui->label->setCurrentState(ui->label->allData);
    clearPredictionChecks();
    ui->actionAll_Data->setChecked(true);
    theDataAnalysis->setPredictionAverage(0);



    updateProfile();


    connect(entryWindow,SIGNAL(addNewEntry(float,QDateTime,unsigned int, unsigned int)),this,SLOT(gettingNewEntry(float,QDateTime,unsigned int,unsigned int)));
    connect(newProfile,SIGNAL(newProfile(Profile)),this,SLOT(gettingNewProfile(Profile)));
    connect(newSavedData,SIGNAL(saveButtonPressed()),this,SLOT(on_actionSave_Profile_triggered()));
    connect(newSavedData,SIGNAL(dontSaveButtonPressed()),this,SLOT(dontSaveAndExit()));
    connect(newSavedData,SIGNAL(cancelButtonPressed()),this,SLOT(cancelExit()));

}

MainWindow::~MainWindow()
{
    delete listOfEntries;
    delete profile;
    delete newProfile;
    delete entryWindow;
    delete newSavedData;
    delete ui;
}

void MainWindow::gettingNewEntry(float inWeight,QDateTime inDateTime,unsigned int inCaloriesConsumed,unsigned int inCaloriesBurned)
{
    Entry newEntry(inWeight,inDateTime,inCaloriesConsumed,inCaloriesBurned);
    listOfEntries->push_back(newEntry);
    updateEntries();
    dataChanged = true;
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
    listOfEntries->clear();
    updateEntries();
    dataChanged = true;
    currentFile = QString("");
    saveAs = true;
    closingProgram = false;
    wantNewProfile = false;
    loadProfile = false;
    closeProfile = false;
    disableAll(false);
}

void MainWindow::on_newEntryButton_clicked()
{
    entryWindow->clearEntry();
    entryWindow->show();
}

void MainWindow::on_actionExit_triggered()
{
    saveAs = false;
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
        dataChanged = true;
    }
}

void MainWindow::dontSaveAndExit()
{
    dataChanged = false;
    newSavedData->hide();
    if(wantNewProfile)
    {
        this->on_actionNew_Profile_triggered();
    }
    else if(loadProfile)
    {
        this->on_actionOpen_Projile_triggered();
    }
    else if(closeProfile)
    {
        this->on_actionClose_Profile_triggered();
    }
    else
    {
        this->close();
    }
}

void MainWindow::cancelExit()
{
    newSavedData->hide();
    closingProgram = false;
    wantNewProfile = false;
    loadProfile = false;
    closeProfile = false;
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
    ui->unnAccCaloriesLabel->setText(QString::number(theDataAnalysis->getUnaccountedForCalories()));
    ui->weightRangeLabel->setText(QString::number(floor(theDataAnalysis->getWeightRange()*10)/10) + QString(" Lbs"));
    QDate predictionTargetDate = theDataAnalysis->getDateOfGoal();
    ui->estDateWeightLossLabel->setText(predictionTargetDate.toString());
    ui->estFatBurnedLabel->setText(QString::number(floor(theDataAnalysis->getEstimatedFatBurned()*10)/10) + QString(" Lbs"));
    if(listOfEntries->size() > 0)
    {
        ui->totalWeightLossLabel->setText(QString::number(profile->getInitialWeight() - listOfEntries->at(listOfEntries->size() - 1).getWeight()));

    }
}

void MainWindow::on_actionNew_Profile_triggered()
{
    if(dataChanged)
    {
        wantNewProfile = true;
        newSavedData->show();
    }
    else
    {
        newProfile->show();
    }
}

void MainWindow::on_actionClose_Profile_triggered()
{
    if(dataChanged)
    {
        closeProfile = true;
        newSavedData->show();
    }
    else
    {
        loadProfile = false;
        closingProgram = false;
        wantNewProfile = false;
        closeProfile = false;
        currentFile = QString("");
        saveAs = true;
        if(profile != 0)
        {
            delete profile;
            profile = 0;
        }
        profile = new Profile(QString(""),QDate(1970,1,01),60,1,100,100);
        updateProfile();
        listOfEntries->clear();
        updateEntries();
        dataChanged = false;
        disableAll(true);
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    closingProgram = true;
    if(dataChanged)
    {
        newSavedData->show();
        event->ignore();
    }
    else
    {
        event->accept();
    }


}
void MainWindow::disableAll(bool inDisable)
{
    ui->newEntryButton->setDisabled(inDisable);
    ui->removeEntryButton->setDisabled(inDisable);
    ui->targetWeightEditBox->setDisabled(inDisable);
    ui->weightRangeCheckbox->setDisabled(inDisable);
    ui->predictionCheckbox->setDisabled(inDisable);
    ui->targetWeightCheckBox->setDisabled(inDisable);
    ui->predefinedAutosComboBox->setDisabled(inDisable);
    ui->pushButton->setDisabled(inDisable);
}

void MainWindow::saveFile()
{
    QString fileName;
    if(currentFile.compare(QString("")) == 0 || saveAs)
    {
        fileName = QFileDialog::getSaveFileName(this,tr("Save File"));
    }
    else
    {
        fileName = currentFile;
    }
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
        currentFile = fileName;
        dataChanged = false;
        newSavedData->hide();
        if(closingProgram)
        {
            this->close();
        }
        if(wantNewProfile)
        {
            this->on_actionNew_Profile_triggered();
        }
        if(loadProfile)
        {
            this->on_actionOpen_Projile_triggered();
        }
        if(closeProfile)
        {
            this->on_actionClose_Profile_triggered();
        }
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
            currentFile = fileName;
            loadProfile = false;
            closingProgram = false;
            wantNewProfile = false;
            dataChanged = false;
            closeProfile = false;
            disableAll(false);
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
                        theDataAnalysis->updateVariables();
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
    theDataAnalysis->setProfile(profile);
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
    saveAs = false;
    saveFile();
}

void MainWindow::on_actionSave_Profile_as_triggered()
{
    saveAs = true;
    saveFile();
}


void MainWindow::on_actionOpen_Projile_triggered()
{
    if(dataChanged)
    {
        loadProfile = true;
        newSavedData->show();
    }
    else
    {
        loadFile();
    }
}

void MainWindow::on_pushButton_clicked()
{
    profile->setTargetWeight(ui->targetWeightEditBox->text().toFloat());
    QDate predictionTargetDate = theDataAnalysis->getDateOfGoal();
    ui->estDateWeightLossLabel->setText(predictionTargetDate.toString());
    ui->label->update();
}


void MainWindow::on_weightRangeCheckbox_clicked(bool checked)
{
    ui->label->setShowWeightRange(checked);
    ui->label->update();
}



void MainWindow::on_targetWeightCheckBox_clicked(bool checked)
{
    ui->label->setShowTargetWeight(checked);
    ui->label->update();
}

void MainWindow::on_predictionCheckbox_clicked(bool checked)
{
    ui->label->setShowPrediction(checked);
    ui->label->update();

}

/*ui->predefinedAutosComboBox->addItem(QString("All Data"));
ui->predefinedAutosComboBox->addItem(QString("Past Week"));
ui->predefinedAutosComboBox->addItem(QString("Past 2 Weeks"));
ui->predefinedAutosComboBox->addItem(QString("Past 3 Weeks"));
ui->predefinedAutosComboBox->addItem(QString("Past Month"));
ui->predefinedAutosComboBox->addItem(QString("Full View"));
ui->predefinedAutosComboBox->addItem(QString("Custom"));
*/

void MainWindow::on_predefinedAutosComboBox_currentIndexChanged(const QString &arg1)
{
    if(QString("All Data").compare(arg1) == 0)
    {
        ui->label->setCurrentState(ui->label->allData);
    }
    else if(QString("Past Week").compare(arg1) == 0)
    {
        ui->label->setCurrentState(ui->label->pastWeek);
    }
    else if(QString("Past 2 Weeks").compare(arg1) == 0)
    {
        ui->label->setCurrentState(ui->label->pastTwoWeeks);
    }
    else if(QString("Past 3 Weeks").compare(arg1) == 0)
    {
        ui->label->setCurrentState(ui->label->pastThreeWeeks);
    }
    else if(QString("Past Month").compare(arg1) == 0)
    {
        ui->label->setCurrentState(ui->label->pastMonth);
    }
    else if(QString("Full View").compare(arg1) == 0)
    {
        ui->label->setCurrentState(ui->label->fullView);
    }
    ui->label->updateVariables();
    ui->label->update();
}

void MainWindow::on_actionAll_Data_triggered()
{
    clearPredictionChecks();
    ui->actionAll_Data->setChecked(true);
    theDataAnalysis->setPredictionAverage(0);
    ui->label->updateVariables();
    ui->label->update();
    on_pushButton_clicked();
}

void MainWindow::on_actionmonth_triggered()
{
    clearPredictionChecks();
    ui->actionmonth->setChecked(true);
    theDataAnalysis->setPredictionAverage(1);
    ui->label->updateVariables();
    ui->label->update();
    on_pushButton_clicked();
}

void MainWindow::on_action3_weeks_triggered()
{
    clearPredictionChecks();
    ui->action3_weeks->setChecked(true);
    theDataAnalysis->setPredictionAverage(2);
    ui->label->updateVariables();
    ui->label->update();
    on_pushButton_clicked();
}

void MainWindow::on_action2_weeks_triggered()
{
    clearPredictionChecks();
    ui->action2_weeks->setChecked(true);
    theDataAnalysis->setPredictionAverage(3);
    ui->label->updateVariables();
    ui->label->update();
    on_pushButton_clicked();
}

void MainWindow::on_actionPast_week_triggered()
{
    clearPredictionChecks();
    ui->actionPast_week->setChecked(true);
    theDataAnalysis->setPredictionAverage(4);
    ui->label->updateVariables();
    ui->label->update();
    on_pushButton_clicked();
}

void MainWindow::on_actionDay_triggered()
{
    clearPredictionChecks();
    ui->actionDay->setChecked(true);
    theDataAnalysis->setPredictionAverage(5);
    ui->label->updateVariables();
    ui->label->update();
    on_pushButton_clicked();
}

void MainWindow::clearPredictionChecks()
{
    ui->actionDay->setChecked(false);
    ui->actionPast_week->setChecked(false);
    ui->action2_weeks->setChecked(false);
    ui->action3_weeks->setChecked(false);
    ui->actionmonth->setChecked(false);
    ui->actionAll_Data->setChecked(false);
}

