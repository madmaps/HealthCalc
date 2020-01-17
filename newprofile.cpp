#include "newprofile.h"
#include "ui_newprofile.h"

NewProfile::NewProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProfile)
{
    ui->setupUi(this);
    ui->genderComboBox->addItem(QString("Male"));
    ui->genderComboBox->addItem(QString("Female"));
}

NewProfile::~NewProfile()
{
    delete ui;
}

void NewProfile::on_pushButton_clicked()
{
    bool theGender = false;
    QString genderCheck("Male");
    if(genderCheck.compare(ui->genderComboBox->currentText()) == 0)
    {
        theGender = true;
    }
    Profile theProfile(ui->nameLineEdit->text(),ui->dateOfBirthLineEdit->date(),ui->heightSpinBox->value(),theGender,ui->initialWeightLineEdit->text().toFloat(),ui->targetWeightLineEdit->text().toFloat());
    hide();
    newProfile(theProfile);
}
