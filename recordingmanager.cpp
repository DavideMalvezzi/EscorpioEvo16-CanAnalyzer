#include "recordingmanager.h"
#include "recordingdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QSqlRecord>
#include <QTime>
#include <QSqlError>

RecordingManager::RecordingManager(QObject *parent) : QThread(parent){
    db = QSqlDatabase::addDatabase("QSQLITE", "EscorpioDB");
    sqlModel = new QSqlTableModel(this, db);
    sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    connect(&saveTimer, SIGNAL(timeout()), this, SLOT(submitOnDB()));
    connect(this, SIGNAL(addToSaveQueue(unsigned short,QString)), this, SLOT(saveOnDB(unsigned short,QString)),
            Qt::QueuedConnection);

    moveToThread(this);
}

bool RecordingManager::startRecording(QString dbPath){
    db.setDatabaseName(dbPath);

    if(QFile(dbPath).exists() && db.open()){
        RecordingDialog d(db);
        if(d.exec()){
            raceID = d.getRaceID();

            sqlModel->setTable("Data");

            saveTimer.setInterval(10000);
            saveTimer.start();
            return true;
        }
        return false;
    }
    QMessageBox::critical(NULL, "Error", "Database file not found: impossible to record");
    return false;
}

void RecordingManager::stopRecording(){
    QSqlQuery q(db);
    q.prepare("UPDATE Race SET EndTime = :time WHERE IDRace = :id");
    q.bindValue(":time", QTime::currentTime().toString("hh:mm:ss:zzz"));
    q.bindValue(":id", raceID);
    q.exec();

    db.close();
    QSqlDatabase::removeDatabase("EscorpioDB");

    saveTimer.stop();
}

void RecordingManager::saveOnDB(unsigned short id, QString result){
    //qDebug()<<"save "<<QThread::currentThreadId();

    int index = channelIndex.value(id, 0);
    QSqlRecord record = sqlModel->record();

    record.setValue("Channel", id);
    record.setValue("Race", raceID);
    record.setValue("Index", index);
    record.setValue("Time", QTime::currentTime().toString("hh:mm:ss.zzz"));
    record.setValue("Value", result);
    sqlModel->insertRecord(0, record);
    sqlModel->selectRow(0);

    channelIndex.insert(id, ++index);
}

void RecordingManager::submitOnDB(){
    //qDebug()<<"submit "<<QThread::currentThreadId();

    sqlModel->database().transaction();
    sqlModel->submitAll();
    sqlModel->database().commit();
    sqlModel->lastError().text();
    sqlModel->clear();
    sqlModel->setTable("Data");
}
