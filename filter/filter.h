#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QMessageBox>

#define RANGE_FILTER_TYPE   0
#define LIST_FILTER_TYPE    1

class Filter : public QObject {
    Q_OBJECT
public:
    explicit Filter(QObject *parent = 0) : QObject(parent){}
    virtual bool matchFilter(unsigned int id) = 0;
    virtual int type() = 0;
};

//Range filter
class RangeFilter : public Filter {

public:
    RangeFilter(unsigned int min, unsigned int max){
        this->minID = qMin(min, max);
        this->maxID = qMax(min, max);
    }

    virtual bool matchFilter(unsigned int id){
        return id >= minID && id <= maxID;
    }

    virtual int type(){return RANGE_FILTER_TYPE;}

    unsigned int getMaxID(){return maxID;}
    unsigned int getMinID(){return minID;}

private:
    unsigned int minID, maxID;
};

//List filter
class ListFilter : public Filter {

public:
    ListFilter(QList<unsigned int> list){
        this->listID = list;
    }

    virtual bool matchFilter(unsigned int id){
        return listID.contains(id);
    }

    virtual int type(){return LIST_FILTER_TYPE;}

    QList<unsigned int> getListID(){return listID;}

private:
    QList<unsigned int> listID;
};

//Filter model
class ResponseFilterModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    ResponseFilterModel(QObject* parent) : QSortFilterProxyModel(parent){
        this->filter = NULL;
    }

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
        Q_UNUSED(source_parent);

        if(filter != NULL){
            QModelIndex id = sourceModel()->index(source_row, 0);
            return filter->matchFilter(sourceModel()->data(id).toString().toInt(NULL, 16));
        }

        return true;
    }

    void setFilter(Filter* filter){
        this->filter = filter;
        invalidateFilter();
    }

    Filter* getFilter(){
        return filter;
    }

    void exportToFile(QString filename){
        QFile file(filename);
        QTextStream stream(&file);
        if(file.open(QIODevice::WriteOnly)){

            stream <<
                     "<?xml version=\"1.0\"?><?mso-application progid=\"Excel.Sheet\"?>" << endl <<
                     "<Workbook" << endl <<
                     "xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"" << endl <<
                     "xmlns:o=\"urn:schemas-microsoft-com:office:office\"" << endl <<
                     "xmlns:x=\"urn:schemas-microsoft-com:office:excel\"" << endl <<
                     "xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"" << endl <<
                     "xmlns:html=\"http://www.w3.org/TR/REC-html40\">" << endl;
            stream <<
                     "<DocumentProperties xmlns = \"urn:schemas - microsoft - com:office:office\">" << endl <<
                     "<Author> CAN Analyzer </Author>" << endl <<
                     "<Created>" << QDate::currentDate().toString("yyyy-MM-dd") << "T" <<
                     QTime::currentTime().toString("hh:mm:ss") << "Z</Created>" << endl <<
                     "<Company> TeamZeroC </Company>" << endl <<
                     "<Version> 1.00 </Version>" << endl <<
                     "</DocumentProperties>" << endl;

            stream << "<Worksheet ss:Name=\"CAN Chronology\">" << endl;
            stream << "<Table>" << endl;

            stream << "<Row>" << endl;
            stream << "<Cell><Data ss:Type = \"String\">CAN ID</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">Name</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">Size</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D1</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D2</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D3</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D4</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D5</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D6</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D7</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">D8</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">Value</Data></Cell>" <<endl;
            stream << "<Cell><Data ss:Type = \"String\">Note</Data></Cell>" <<endl;
            stream << "</Row>" << endl;


            for(int i = 0; i < rowCount(); i++){
                stream << "<Row>" << endl;
                for(int j = 0; j< columnCount(); j++){
                    stream << "<Cell><Data ss:Type = \"String\">";
                    stream << data(index(i, j)).toString();
                    stream << "</Data></Cell>" << endl;
                }
                stream << "</Row>" << endl;
            }

            stream << "</Table>" << endl;
            stream << "</Worksheet>" << endl;
            stream << "</Workbook>" << endl;

            file.close();
        }
        else{
            QMessageBox::critical(NULL, "Error", QString("Error saving file: %1").arg(file.errorString()));
        }
    }

private:
    Filter* filter;
};

#endif // FILTER_H
