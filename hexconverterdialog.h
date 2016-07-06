#ifndef HEXCONVERTERDIALOG_H
#define HEXCONVERTERDIALOG_H

#include <QDialog>

namespace Ui {
class HexConverterDialog;
}

class HexConverterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HexConverterDialog(QWidget *parent = 0);
    ~HexConverterDialog();

private slots:
    void on_closeButton_clicked();
    void on_convertButton_clicked();
    void on_typeBox_currentIndexChanged(int index);

    QByteArray floatToHex(float f);
    QByteArray doubleToHex(double f);

private:
    Ui::HexConverterDialog *ui;
};

#endif // HEXCONVERTERDIALOG_H
