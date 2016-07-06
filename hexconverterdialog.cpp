#include "hexconverterdialog.h"
#include "ui_hexconverterdialog.h"

#include <QtMath>
#include <QIntValidator>
#include <QDoubleValidator>
#include "binaryvalidator.h"
#include "hexvalidator.h"

HexConverterDialog::HexConverterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HexConverterDialog){
    ui->setupUi(this);
    ui->valueLine->setValidator(new BinaryValidator(this));
}

HexConverterDialog::~HexConverterDialog(){
    delete ui;
}

void HexConverterDialog::on_closeButton_clicked(){
    reject();
}

void HexConverterDialog::on_convertButton_clicked(){
    QString temp;
    QByteArray bytes;
    switch (ui->typeBox->currentIndex()) {
        case 0:
           temp = QString::number(ui->valueLine->text().toInt(NULL, 2), 16);
        break;
        case 1:
           bytes.setNum(ui->valueLine->text().toShort(),16);
        break;
        case 2:
           bytes.setNum(ui->valueLine->text().toInt(),16);
        break;
        case 3:
           bytes.setNum((qlonglong)ui->valueLine->text().toLong(),16);
        break;
        case 4:
           bytes = floatToHex(ui->valueLine->text().toFloat());
        break;
        case 5:
           bytes = doubleToHex(ui->valueLine->text().toFloat());
        break;
        case 6:
           temp = QString(ui->valueLine->text().toLatin1().toHex());
        break;
    }

    if(bytes.size() % 2 == 1){
        bytes.prepend('0');
    }

    for(int i = 0; i < bytes.size(); i += 2){
        temp.prepend(bytes.at(i+1));
        temp.prepend(bytes.at(i));
    }

    ui->hexLine->setText(temp);

}

QByteArray HexConverterDialog::floatToHex(float f){
    unsigned long long int buf = *(reinterpret_cast<unsigned long int*>(&f));
    return QString::number(buf, 16).toLatin1();
}

QByteArray HexConverterDialog::doubleToHex(double f){
    unsigned long long int buf = *(reinterpret_cast<unsigned long long int*>(&f));
    return QString::number(buf, 16).toLatin1();
}


void HexConverterDialog::on_typeBox_currentIndexChanged(int index){
    ui->valueLine->setMaxLength(64);
    ui->valueLine->clear();
    switch (index) {
        case 0:
            ui->valueLine->setValidator(new BinaryValidator(this));
        break;
        case 1:
            ui->valueLine->setValidator(new QIntValidator(-qPow(2, 15), qPow(2, 15)-1, this));
        break;
        case 2:
            ui->valueLine->setValidator(new QIntValidator(-qPow(2, 31), qPow(2, 31)-1, this));
        break;
        case 3:
        ui->valueLine->setValidator(new QIntValidator(-qPow(2, 63), qPow(2, 63)-1, this));
        break;
        case 4:
        case 5:
            ui->valueLine->setValidator(new QDoubleValidator(this));
        break;
        case 6:
            ui->valueLine->setMaxLength(8);
            ui->valueLine->setValidator(NULL);
        break;
    }
}
