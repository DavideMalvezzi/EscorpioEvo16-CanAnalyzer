#include "chronomodel.h"

#include <QFile>
#include <QDate>
#include <QTime>

ChronoModel::ChronoModel(QObject* parent): QAbstractTableModel(parent){

}

Qt::ItemFlags ChronoModel::flags(const QModelIndex & /*index*/) const{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled ;
}

QVariant ChronoModel::data(const QModelIndex &index, int role) const{
    int row = index.row();
    int column = index.column();

    if (role == Qt::DisplayRole && row < converterResponses.size()){
        ConversionResponse* r = converterResponses.at(row);
        QString byte;
        switch (column){
            case 0:
                return QString("0x%1").arg(QString::number(r->id, 16));
            case 1:
                return r->channel == NULL ? "UNKNOWN" : r->channel->name;
            case 2:
                return r->data.size();
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                if(column - 3 < r->data.size()){
                    byte = QString::number((unsigned char)r->data.at(column-3), 16).toUpper();
                    return byte.size() < 2 ? byte.prepend('0') : byte;
                }
                return QVariant();
            case 11:
                return r->result;
            case 12:
                return r->note;
        }
    }
    else if(role == Qt::TextAlignmentRole){
        return column < 11 ? QVariant(Qt::AlignHCenter | Qt::AlignVCenter) : QVariant(Qt::AlignHCenter);
    }

    return QVariant();
}

QVariant ChronoModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section){
                case 0:
                    return QString("CAN ID");
                case 1:
                    return QString("Name");
                case 2:
                    return QString("Size");
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                    return QString("D%1").arg(section - 2);
                case 11:
                    return QString("Value");
                case 12:
                    return QString("Notes");
            }
        }
    }
    return QVariant();
}

void ChronoModel::addResponse(ConversionResponse *response){
    emit layoutAboutToBeChanged();
    converterResponses.append(response);
    emit layoutChanged();
}

void ChronoModel::clear(){
    emit layoutAboutToBeChanged();
    while(!converterResponses.isEmpty()){
        delete converterResponses.takeFirst();
    }
    emit layoutChanged();
}



