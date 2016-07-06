#ifndef STREAMINGMANAGER_H
#define STREAMINGMANAGER_H

#include <QUdpSocket>
#include <QThread>
#include <QNetworkAddressEntry>

class StreamingManager : public QThread {
    Q_OBJECT

public:
    StreamingManager(QObject* parent=0);
    void setPort(int port){this->port = port;}


signals:
    void addToStreamQueue(QByteArray);

public slots:
    void sendPacket(QByteArray);

private:
    int port;
    QUdpSocket* socket;
    QList<QHostAddress> availableBroadcast;
};

#endif // STREAMINGMANAGER_H
