#include "caninterface.h"

#include <QDebug>
#include <QMessageBox>

CanInterface::CanInterface(QObject* parent) : QThread(parent){
    this->port = new QSerialPort(this);
    this->packetCount = 0;
    this->isPaused = false;

    connect(port, SIGNAL(readyRead()), this, SLOT(onDataReceived()), Qt::QueuedConnection);
}

bool CanInterface::play(){
    if(isPaused && port->isOpen()){
        isPaused = false;
    }
    else{
        port->setDataTerminalReady(false); //needed for native usb arduino due
        if(port->open(QSerialPort::ReadWrite)){
            isPaused = false;
            port->setDataTerminalReady(true); //needed for native usb arduino due
        }
        else{
            QMessageBox::critical(NULL, "Error", QString("Error on opening the serial port: %1").arg(port->errorString()));
            return false;
        }
    }

    return true;
}

void CanInterface::pause(){
    isPaused = true;
}

void CanInterface::stop(){
    port->close();
}

void CanInterface::onDataReceived(){
    //qDebug()<<"serial "<<QThread::currentThreadId();

    if(!isPaused){
        rxBuffer.append(port->readAll());

        int index = rxBuffer.indexOf(CAN_PACKET_HEADER);
        unsigned short id, size;
        QByteArray data;

        while(index != -1){
            index += 3;
            if(rxBuffer.size() >= index + 3){
                id = (((rxBuffer.at(index) & 0xFF) << 8) | (rxBuffer.at(index + 1) & 0xFF));
                size = rxBuffer.at(index+2);
                index += 3;
                if(rxBuffer.size() >= index + size){
                    for(int i = 0; i < size; i++){
                        data.append(rxBuffer.at(index + i));
                    }

                    rxBuffer.remove(0, index + size);
                    qDebug() << "received" << id << "size " << size << "data" << data.toHex().toStdString().c_str();
                    packetCount++;
                    emit packetReceived(id, data);

                    index = rxBuffer.indexOf(CAN_PACKET_HEADER);
                    data.clear();
                }else{
                    index = -1;
                }
            }else{
                index = -1;
            }
        }
    }else{
        rxBuffer.clear();
        port->readAll();
    }

}

bool CanInterface::sendMsg(unsigned int id, QByteArray data){
    if(port != NULL && port->isOpen()){
        port->write("CAN");
        port->putChar(id >> 8);
        port->putChar(id);
        port->putChar(data.size());
        port->write(data);

        if(!port->waitForBytesWritten(3000)){
            QMessageBox::critical(NULL, "Error", "Send error: send timeout");
            return false;
        }
        return true;
    }

    QMessageBox::critical(NULL, "Error", "Send error: serial port is closed");
    return false;
}
