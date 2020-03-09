#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "addentry.h"
#include "entry.h"
#include "newprofile.h"
#include "profile.h"
#include "dataanalysis.h"

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

private:
    void updateEntries();

    void disableAll(bool inDisable);

    void saveFile();

    void loadFile();

    void updateProfile();

    Ui::MainWindow *ui;
    AddEntry* entryWindow;
    std::vector<Entry>* listOfEntries;
    NewProfile* newProfile;
    DataAnalysis* theDataAnalysis;
    bool profileLoaded;
    Profile* profile;

};
#endif // MAINWINDOW_H
