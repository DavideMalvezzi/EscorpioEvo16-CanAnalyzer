#ifndef RECORDINGDIALOG_H
#define RECORDINGDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
    class RecordingDialog;
}

class RecordingDialog : public QDialog {
    Q_OBJECT

public:
    explicit RecordingDialog(QSqlDatabase& db, QWidget *parent = 0);
    int getRaceID(){ return raceID;}
    ~RecordingDialog();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();

private:
    Ui::RecordingDialog *ui;
    int raceID;
    QSqlDatabase db;
};

#endif // RECORDINGDIALOG_H
