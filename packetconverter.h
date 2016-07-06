#ifndef PACKETCONVERTER_H
#define PACKETCONVERTER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QThread>

#include "muparser/muParser.h"
using namespace mu;

#define BITFLAG     'B'
#define U_NUMBER    'U'
#define NUMBER      'I'
#define STRING      'S'
#define DECIMAL     'D'

class PacketConverter;

//Channel
class Channel {

public:
    unsigned short id, size;
    char dataType;
    QString name, formula;
};

class ConversionResponse {

public:
    unsigned short id;
    QByteArray data;
    Channel* channel;
    QString result, note;
};


//Conversion thread
class ConversionThread : public QThread {
    Q_OBJECT

public:
    ConversionThread(PacketConverter* convert);
    void set(unsigned short id, QByteArray data, Channel* channel, unsigned short dataType, QString formula);
    virtual void run();

signals:
    void conversionFinished(ConversionResponse* response);

private:
    unsigned short id, dataType;
    QByteArray data;
    Channel* channel;
    QString formula;

    QString convertToBitFlag(QByteArray& data);
    double convertToUNumber(QByteArray& data);
    double convertToNumber(QByteArray& data);
    double convertToFloat(QByteArray& data);
    double convertToDouble(QByteArray& data);

};

//Converter
class PacketConverter : public QObject {
    Q_OBJECT

public:
    explicit PacketConverter(QString dbPath, QObject *parent = 0);
    Channel* getChannelProp(unsigned short id){return channels.value(id, NULL);}
    QMap<unsigned short, Channel*> getChannelsProps(){return channels;}

public slots:
    void onThreadFinished();
    void convertPacket(unsigned short id, QByteArray data);

    int getTotalThread(){return createdT;}
    int getUsedThread(){return createdT - threadsPool.size();}


signals:
    void packetConverted(ConversionResponse* response);

private:
    int createdT;
    QList<ConversionThread*> threadsPool;
    QMap<unsigned short, Channel*> channels;
};



#endif // PACKETCONVERTER_H
