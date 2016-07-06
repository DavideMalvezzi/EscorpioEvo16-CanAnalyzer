#include "createfilterdialog.h"
#include "ui_createfilterdialog.h"

#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>

CreateFilterDialog::CreateFilterDialog(Filter* f, QList<Channel*> channels, QWidget *parent) :
    QDialog(parent), ui(new Ui::CreateFilterDialog){

    ui->setupUi(this);
    this->filter = f;

    for(int i = 0; i < channels.size(); i++){
        ui->minIDCombo->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->maxIDCombo->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->idCombo->addItem(channels.at(i)->name, channels.at(i)->id);
    }

    QSignalMapper* mapper = new QSignalMapper(this);
    mapper->setMapping(ui->minIDCombo, ui->minIDSpin);
    mapper->setMapping(ui->maxIDCombo, ui->maxIDSpin);
    mapper->setMapping(ui->idCombo, ui->idSpin);

    connect(ui->minIDCombo, SIGNAL(activated(int)), mapper, SLOT(map()));
    connect(ui->maxIDCombo, SIGNAL(activated(int)), mapper, SLOT(map()));
    connect(ui->idCombo, SIGNAL(activated(int)), mapper, SLOT(map()));

    connect(mapper, SIGNAL(mapped(QWidget*)), this, SLOT(selectedKnownChannel(QWidget*)));

    //Load current filter
    if(f != NULL){
        RangeFilter* rf;
        ListFilter* lf;
        switch (f->type()) {
            case RANGE_FILTER_TYPE:
                rf = (RangeFilter*)f;
                ui->minIDSpin->setValue(rf->getMinID());
                ui->maxIDSpin->setValue(rf->getMaxID());
                ui->rangeFilterRadio->setChecked(true);
            break;

            case LIST_FILTER_TYPE:
                lf = (ListFilter*)f;
                for(int i = 0; i < lf->getListID().size(); i++){
                    ui->idList->addItem(QString("0x%1").arg(QString::number(lf->getListID().at(i),16)));
                }
                ui->listFilterRadio->setChecked(true);
            break;
        }
    }else{
        ui->noFilterRadio->setChecked(true);
    }
}

CreateFilterDialog::~CreateFilterDialog(){
    delete ui;
}

void CreateFilterDialog::on_okButton_clicked(){
    if(ui->noFilterRadio->isChecked()){
        filter = NULL;
    }
    if(ui->rangeFilterRadio->isChecked()){
        filter = new RangeFilter(ui->minIDSpin->value(), ui->maxIDSpin->value());
        if(ui->minIDSpin->value() > ui->maxIDSpin->value()){
            QMessageBox::warning(this, "Warning", "MinID > MaxID: values will be switched");
        }
    }
    else if(ui->listFilterRadio->isChecked()){
        QList<unsigned int> idList;

        for(int i = 0; i < ui->idList->model()->rowCount(); i++){
            idList.append(ui->idList->item(i)->text().toInt(NULL, 16));
        }

        if(idList.size() == 0){
            QMessageBox::critical(this, "Error", "CAN ID list is empty");
            return;
        }
        filter = new ListFilter(idList);
    }
    accept();
}

void CreateFilterDialog::on_cancelButton_clicked(){
    reject();
}

void CreateFilterDialog::on_removeFromListButton_clicked(){
    delete ui->idList->takeItem(ui->idList->currentIndex().row());
}

void CreateFilterDialog::on_addToListButton_clicked(){
    QString channelID = QString("0x%1").arg(QString::number(ui->idSpin->value(),16));
    if(ui->idList->findItems(channelID,Qt::MatchFixedString).size() == 0){
        ui->idList->insertItem(ui->idList->model()->rowCount(), new QListWidgetItem(channelID));
    }
}

void CreateFilterDialog::selectedKnownChannel(QWidget* dest){
    QSpinBox* d = (QSpinBox*)dest;
    QComboBox* s = (QComboBox*)(((QSignalMapper*)QObject::sender())->mapping(dest));
    d->setValue(s->currentData().toInt());
}
