#include "newprofile.h"
#include "ui_newprofile.h"

NewProfile::NewProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProfile)
{
    ui->setupUi(this);
}

NewProfile::~NewProfile()
{
    delete ui;
}
