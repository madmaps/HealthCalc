#ifndef NEWPROFILE_H
#define NEWPROFILE_H

#include <QDialog>
#include "profile.h"

namespace Ui {
class NewProfile;
}

class NewProfile : public QDialog
{
    Q_OBJECT

public:
    explicit NewProfile(QWidget *parent = nullptr);
    ~NewProfile();

signals:
    void newProfile(Profile outProfile);

private slots:
    void on_pushButton_clicked();

private:
    Ui::NewProfile *ui;
};

#endif // NEWPROFILE_H
