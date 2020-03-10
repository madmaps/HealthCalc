#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <QDialog>

namespace Ui {
class SaveData;
}

class SaveData : public QDialog
{
    Q_OBJECT

public:
    explicit SaveData(QWidget *parent = nullptr);
    ~SaveData();

private slots:
    void on_saveButton_clicked();

    void on_dontSaveButton_clicked();

    void on_cancelButton_clicked();

signals:
    void saveButtonPressed();

    void dontSaveButtonPressed();

    void cancelButtonPressed();

private:
    Ui::SaveData *ui;
};

#endif // SAVEDATA_H
