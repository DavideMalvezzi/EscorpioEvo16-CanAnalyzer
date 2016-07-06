#ifndef CHRONOMODEL_H
#define CHRONOMODEL_H

#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QTableView>

#include "packetconverter.h"

class ChronoModel : public QAbstractTableModel {
    Q_OBJECT

public:
    ChronoModel(QObject* parent);
    int columnCount(const QModelIndex &parent) const{Q_UNUSED(parent); return 13;}
    int rowCount(const QModelIndex &parent) const{Q_UNUSED(parent); return converterResponses.size();}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & /*index*/) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addResponse(ConversionResponse* response);
    void clear();

private:
    QList<ConversionResponse*> converterResponses;
};

//Delegate
class PlainStyleDelegate: public QItemDelegate{

public:
    PlainStyleDelegate(QTableView* table) : QItemDelegate(){
        this->table = table;

    }

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        int top = table->rowAt(0);
        int bottom = table->rowAt(table->height() - table->rowHeight(0));
        bottom = bottom == -1? table->model()->rowCount() : bottom;

        int row = index.row();

        if(row >= top && row <= bottom){
            QStyleOptionToolButton label;
            label.rect = option.rect;
            label.text = index.data().toString();
            QApplication::style()->drawControl(QStyle::CE_ToolButtonLabel, &label, painter);
        }
    }

private:
    QTableView* table;
};

#endif // CHRONOMODEL_H
