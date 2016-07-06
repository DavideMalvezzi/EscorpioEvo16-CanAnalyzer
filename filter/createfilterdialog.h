#ifndef CREATEFILTERDIALOG_H
#define CREATEFILTERDIALOG_H

#include <QDialog>
#include "filter.h"
#include "packetconverter.h"

namespace Ui {
    class CreateFilterDialog;
}

class CreateFilterDialog : public QDialog{
    Q_OBJECT

public:
    explicit CreateFilterDialog(Filter* filter, QList<Channel*> channels, QWidget *parent = 0);
    Filter* getFilter(){return filter;}
    ~CreateFilterDialog();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_addToListButton_clicked();
    void on_removeFromListButton_clicked();

    void selectedKnownChannel(QWidget*);

private:
    Ui::CreateFilterDialog *ui;
    Filter* filter;
};

#endif // CREATEFILTERDIALOG_H
