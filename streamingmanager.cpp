#include "streamingmanager.h"

StreamingManager::StreamingManager(QObject* parent) : QThread(parent){
    this->port = port;
    this->socket = new QUdpSocket(this);

    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

    for (int i = 0; i < ifaces.size(); i++) {
        QList<QNetworkAddressEntry> addrs = ifaces[i].addressEntries();

        for (int j = 0; j < addrs.size(); j++){
            if ((addrs[j].ip().protocol() == QAbstractSocket::IPv4Protocol) && (addrs[j].broadcast().toString() != "")){
                availableBroadcast.append(addrs[j].broadcast());
            }
        }
    }

    connect(this, SIGNAL(addToStreamQueue(QByteArray)), this, SLOT(sendPacket(QByteArray)), Qt::QueuedConnection);

    moveToThread(this);
}

void StreamingManager::sendPacket(QByteArray packet){
    //qDebug()<<"streaming "<<QThread::currentThreadId();

    for(int i = 0; i < availableBroadcast.size(); i++){
        socket->writeDatagram(packet, packet.size(), availableBroadcast.at(i), port);
    }
}

