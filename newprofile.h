#ifndef NEWPROFILE_H
#define NEWPROFILE_H

#include <QDialog>

namespace Ui {
class NewProfile;
}

class NewProfile : public QDialog
{
    Q_OBJECT

public:
    explicit NewProfile(QWidget *parent = nullptr);
    ~NewProfile();

private:
    Ui::NewProfile *ui;
};

#endif // NEWPROFILE_H
