#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "addentry.h"
#include "entry.h"
#include "newprofile.h"
#include "profile.h"
#include "dataanalysis.h"
#include "savedata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newEntryButton_clicked();

    void on_actionExit_triggered();

    void gettingNewEntry(float inWeight,QDateTime inDateTime,unsigned int inCaloriesConsumed,unsigned int inCaloriesBurned);

    void gettingNewProfile(Profile inProfile);

    void on_removeEntryButton_clicked();

    void on_actionNew_Profile_triggered();

    void on_actionSave_Profile_triggered();

    void on_actionOpen_Projile_triggered();

    void on_pushButton_clicked();

    void on_weightRangeCheckbox_clicked(bool checked);

    void on_predictionCheckbox_clicked(bool checked);

    void on_targetWeightCheckBox_clicked(bool checked);

    void on_predefinedAutosComboBox_currentIndexChanged(const QString &arg1);

    void on_actionSave_Profile_as_triggered();

    void dontSaveAndExit();

    void cancelExit();

    void on_actionClose_Profile_triggered();

    void on_actionPast_week_triggered();

    void on_actionAll_Data_triggered();

    void on_actionmonth_triggered();

    void on_action3_weeks_triggered();

    void on_action2_weeks_triggered();

    void on_actionDay_triggered();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void updateEntries();

    void disableAll(bool inDisable);

    void saveFile();

    void loadFile();

    void updateProfile();

    void clearPredictionChecks();

    Ui::MainWindow *ui;
    AddEntry* entryWindow;
    std::vector<Entry>* listOfEntries;
    NewProfile* newProfile;
    SaveData* newSavedData;
    DataAnalysis* theDataAnalysis;
    QString currentFile;
    bool profileLoaded;
    Profile* profile;
    bool dataChanged;
    bool closingProgram;
    bool saveAs;
    bool wantNewProfile;
    bool loadProfile;
    bool closeProfile;

};
#endif // MAINWINDOW_H
