#ifndef RECORDINGMANAGER_H
#define RECORDINGMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QThread>
#include <QTimer>
#include <QMap>

class RecordingManager : public QThread{
    Q_OBJECT

public:
    RecordingManager(QObject *parent = 0);
    bool startRecording(QString dbFile);
    void stopRecording();

private slots:
    void saveOnDB(unsigned short id, QString result);
    void submitOnDB();

signals:
    void addToSaveQueue(unsigned short id, QString result);

private:
    int raceID;
    QTimer saveTimer;
    QSqlDatabase db;
    QSqlTableModel* sqlModel;
    QMap<unsigned short, int> channelIndex;

};

#endif // RECORDINGMANAGER_H
