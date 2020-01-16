#ifndef ADDENTRY_H
#define ADDENTRY_H

#include <QDialog>
#include "entry.h"

namespace Ui {
class AddEntry;
}

class AddEntry : public QDialog
{
    Q_OBJECT

public:
    explicit AddEntry(QWidget *parent = nullptr);
    ~AddEntry();
    void clearEntry();

signals:
    void addNewEntry(float inWeight,QDateTime inDateTime,unsigned int inCaloriesConsumed,unsigned int inCaloriesBurned);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddEntry *ui;
};

#endif // ADDENTRY_H
