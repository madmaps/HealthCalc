#include "savedata.h"
#include "ui_savedata.h"

SaveData::SaveData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveData)
{
    ui->setupUi(this);
}

SaveData::~SaveData()
{
    delete ui;
}

void SaveData::on_saveButton_clicked()
{
    saveButtonPressed();
}

void SaveData::on_dontSaveButton_clicked()
{
    dontSaveButtonPressed();
}

void SaveData::on_cancelButton_clicked()
{
    cancelButtonPressed();
}
