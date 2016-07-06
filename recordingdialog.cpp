#include "recordingdialog.h"
#include "ui_recordingdialog.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QDate>
#include <QTime>
#include <QMessageBox>

RecordingDialog::RecordingDialog(QSqlDatabase& db, QWidget *parent) : QDialog(parent), ui(new Ui::RecordingDialog){
    this->db = db;
    ui->setupUi(this);

    QSqlQueryModel* vehicleModel = new QSqlQueryModel(this);
    vehicleModel->setQuery("SELECT IDVehicle, Name FROM Vehicle", db);
    ui->vehicleCombo->setModel(vehicleModel);
    ui->vehicleCombo->setModelColumn(1);

    QSqlQueryModel* trackModel = new QSqlQueryModel(this);
    trackModel->setQuery("SELECT IDTrack, Name FROM Track", db);
    ui->trackCombo->setModel(trackModel);
    ui->trackCombo->setModelColumn(1);
}

RecordingDialog::~RecordingDialog() {
    delete ui;
}

void RecordingDialog::on_cancelButton_clicked(){
    raceID = -1;
    reject();
}

void RecordingDialog::on_okButton_clicked() {
    QSqlQuery q(db);
    q.prepare("INSERT INTO Race(Name, Date, StartTime, Description, Vehicle, Track)"
              " VALUES (:name, :date, :time, :desc, :vehicle, :track)");
    q.bindValue(":name", ui->nameText->text());
    q.bindValue(":date", QDate::currentDate().toString("dd/MM/yyyy"));
    q.bindValue(":time", QTime::currentTime().toString("hh:mm:ss.zzz"));
    q.bindValue(":desc", ui->descText->toPlainText());
    q.bindValue(":vehicle", ui->vehicleCombo->model()->data(ui->vehicleCombo->model()->index(ui->vehicleCombo->currentIndex(), 0)));
    q.bindValue(":track", ui->trackCombo->model()->data(ui->trackCombo->model()->index(ui->trackCombo->currentIndex(), 0)));
    if(q.exec()){
        q.exec("SELECT MAX(IDRace) FROM Race");
        q.next();
        raceID = q.value(0).toInt();
        accept();
    }
    else{
        QMessageBox::critical(this, "Error", QString("Save error: %1").arg(q.lastError().text()));
    }


}
