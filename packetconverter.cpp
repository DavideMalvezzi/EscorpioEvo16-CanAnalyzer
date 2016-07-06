#include "packetconverter.h"

#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QBitArray>
#include <QFile>

PacketConverter::PacketConverter(QString dbPath, QObject *parent) : QObject(parent){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "EscorpioDB");
    db.setDatabaseName(dbPath);

    if(QFile(dbPath).exists() && db.open()){
        QSqlQuery q(db);
        Channel* c;
        q.exec("SELECT IDChannel, Name, Size, Formula, DataType FROM Channel");
        while(q.next()){
            c = new Channel;
            c->id =         q.value(0).toUInt();
            c->name =       q.value(1).toString();
            c->size =       q.value(2).toUInt();
            c->formula =    q.value(3).toString();
            c->dataType =   q.value(4).toString().at(0).toLatin1();
            channels.insert(c->id, c);
        }
        db.close();
    }
    else{
        QMessageBox::warning(NULL, "Warning", "Database file not found: the values won't be converted");
    }
    QSqlDatabase::removeDatabase("EscorpioDB");

    createdT = 0;
}

void PacketConverter::convertPacket(unsigned short id, QByteArray data){
    Channel* c;
    ConversionThread* t;

    if(channels.contains(id)){
        c = channels.value(id);
        if(threadsPool.size() > 0){
            t = threadsPool.takeFirst();
        }else{
            t = new ConversionThread(this);
            createdT++;
            //qDebug()<<"Threads "<<createdT;
        }
        t->set(id, data, c, c->dataType, c->formula);
        t->start();
    }
    else{
        ConversionResponse* response = new ConversionResponse;
        response->id = id;
        response->data = data;
        response->channel = NULL;
        response->result = "UNKNOWN";
        response->note = "Unknown channel settings";
        emit packetConverted(response);
    }
}

void PacketConverter::onThreadFinished(){
    ConversionThread* t = (ConversionThread*)QObject::sender();
    threadsPool.append(t);
}

//Conversion thread
ConversionThread::ConversionThread(PacketConverter* converter) : QThread(){
    connect(this, SIGNAL(finished()), converter, SLOT(onThreadFinished()));
    connect(this, SIGNAL(conversionFinished(ConversionResponse*)),
            converter, SIGNAL(packetConverted(ConversionResponse*)),
            Qt::QueuedConnection);
}

void ConversionThread::set(unsigned short id, QByteArray data, Channel* channel, unsigned short dataType, QString formula){
    this->id = id;
    this->data = data;
    this->channel = channel;
    this->dataType = dataType;
    this->formula = formula;
}

void ConversionThread::run(){
    double x;
    Parser parser;
    QString result, notes;

    switch (dataType) {
        case BITFLAG:
            result = convertToBitFlag(data);
        break;

        case STRING:
            result = QString(data).append('\0');
        break;

        default:
            if(dataType == NUMBER){
                x = convertToNumber(data);
            }else if(dataType == U_NUMBER){
                x = convertToUNumber(data);
            }else if(dataType == DECIMAL){
                if(data.size() == sizeof(float)){
                    x = convertToFloat(data);
                }
                else if(data.size() == sizeof(double)){
                    x = convertToDouble(data);
                }
                else{
                   result = "";
                   notes = "Invalid bytes number for a decimal number";
                   break;
                }

            }
            try{
                parser.DefineVar("x", &x);
                parser.SetExpr(formula.toStdString());
                result = QString::number(parser.Eval());
            }catch(Parser::exception_type& e){
                result = "ERROR";
                notes = QString(e.GetMsg().c_str());
            }
        break;
    }

    ConversionResponse* response = new ConversionResponse;
    response->id = id;
    response->data = data;
    response->channel = channel;
    response->result = result;
    response->note = notes;
    emit conversionFinished(response);
}

QString ConversionThread::convertToBitFlag(QByteArray& data){
    QString r;
    for(int i = 0; i < data.size(); i++){
       for(int j = 0; j < 8; j++){
           r.append( (data.at(i) & (1 << (7 - j))) ? '1' : '0');
       }
    }

    return r;
}

double ConversionThread::convertToUNumber(QByteArray& data){
    char mem[sizeof(long long int)];

    for(int i = 0; i < data.size(); i++){
        mem[i] = data.at(i);
    }

    for(int i = data.size(); i < sizeof(long long int); i++){
        mem[i] = 0x00;
    }

    return (double)*(reinterpret_cast<unsigned long long int*>(mem));
}

double ConversionThread::convertToNumber(QByteArray& data){
    char mem[sizeof(long long int)];

    for(int i = 0; i < data.size(); i++){
        mem[i] = data.at(i);
    }

    if(mem[data.size()-1] & (1<<7)){
        for(int i = data.size(); i < sizeof(long long int); i++){
            mem[i] = 0xFF;
        }
    }else{
        for(int i = data.size(); i < sizeof(long long int); i++){
            mem[i] = 0x00;
        }
    }

    return (double)*(reinterpret_cast<long long int*>(mem));

}

double ConversionThread::convertToFloat(QByteArray& data){
    char mem[sizeof(float)];

    for(int i = 0; i < data.size(); i++){
        mem[i] = data.at(i);
    }

    return *(reinterpret_cast<float*>(mem));

}

double ConversionThread::convertToDouble(QByteArray& data){
    char mem[sizeof(double)];

    for(int i = 0; i < data.size(); i++){
        mem[i] = data.at(i);
    }

    return *(reinterpret_cast<double*>(mem));

}
