#include "addentry.h"
#include "ui_addentry.h"

AddEntry::AddEntry(QWidget *parent) : QDialog(parent), ui(new Ui::AddEntry)
{
    ui->setupUi(this);
}

AddEntry::~AddEntry()
{
    delete ui;
}

void AddEntry::clearEntry()
{
    ui->lineEdit->setText(QString(""));
    ui->lineEdit_2->setText(QString(""));
    ui->lineEdit_3->setText(QString(""));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void AddEntry::on_pushButton_clicked()
{
    addNewEntry(ui->lineEdit->text().toFloat(),ui->dateTimeEdit->dateTime(),ui->lineEdit_2->text().toUInt(),ui->lineEdit_3->text().toUInt());
    hide();
}

void AddEntry::on_pushButton_2_clicked()
{
    hide();
}
