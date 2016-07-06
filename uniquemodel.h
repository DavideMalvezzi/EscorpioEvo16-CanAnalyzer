#ifndef UNIQUEMODEL_H
#define UNIQUEMODEL_H

#include <QAbstractTableModel>

#include "packetconverter.h"

class UniqueModel: public QAbstractTableModel {
    Q_OBJECT

public:
    UniqueModel(QObject* parent);
    int columnCount(const QModelIndex &parent) const{Q_UNUSED(parent); return 13;}
    int rowCount(const QModelIndex &parent) const{Q_UNUSED(parent); return converterResponses.size();}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & /*index*/) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addResponse(ConversionResponse* response);
    void clear();

private:
    QMap<unsigned int,ConversionResponse*> converterResponses;

};

#endif // UNIQUEMODEL_H
