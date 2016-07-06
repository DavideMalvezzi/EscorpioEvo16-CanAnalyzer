#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <QThread>
#include <QSerialPort>
#include <QTimer>

#define CAN_PACKET_HEADER   "CAD"

class CanInterface : public QThread {
    Q_OBJECT

public:

    CanInterface(QObject* parent = 0);
    QSerialPort* getPort(){return port;}

public slots:
    void onDataReceived();
    bool sendMsg(unsigned int id, QByteArray data);
    int getPPS(){int r = packetCount; packetCount = 0; return r;}

    bool play();
    void pause();
    void stop();

signals:
    void packetReceived(unsigned short id, QByteArray data);

private:
    bool isPaused;
    int packetCount;
    QSerialPort* port;
    QByteArray rxBuffer;
};

#endif // CANINTERFACE_H
