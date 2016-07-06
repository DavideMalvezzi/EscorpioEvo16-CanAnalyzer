#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QUdpSocket>
#include <QTimer>

#include "caninterface.h"
#include "packetconverter.h"
#include "chronomodel.h"
#include "uniquemodel.h"
#include "streamingmanager.h"
#include "recordingmanager.h"
#include "filter/filter.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionRefresh_triggered();

    void on_actionPlay_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();
    void on_actionRecord_triggered();

    void on_actionChronology_triggered();
    void on_actionUnique_triggered();
    void on_actionClear_triggered();
    void on_actionExport_triggered();

    void on_actionSettings_triggered();

    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_actionFilter_triggered();
    void on_dbPathButton_clicked();

    void onDataReady(ConversionResponse* response);
    void loadSerialPortParams();

    void setKnownChannelFromCombo(QWidget*);
    void setKnownChannelFromSpin(QWidget*);
    void loadTXParams();
    void loadTXSettings();
    void saveTXSettings();

    void sendData(unsigned int id, QString data);

    void on_sendDataButton_29_clicked();
    void on_sendDataButton_20_clicked();
    void on_sendDataButton_21_clicked();
    void on_sendDataButton_22_clicked();
    void on_sendDataButton_23_clicked();
    void on_sendDataButton_24_clicked();
    void on_sendDataButton_25_clicked();
    void on_sendDataButton_26_clicked();
    void on_sendDataButton_27_clicked();
    void on_sendDataButton_28_clicked();

    void on_actionStartStreaming_triggered();
    void on_actionEndStreaming_triggered();
    void sendOverNetwork(ConversionResponse*);

    void showStats();


    void on_actionHexConverter_triggered();

private:
    bool streamEnabled, recordEnabled;
    Ui::MainWindow *ui;
    QComboBox* portCombo;
    CanInterface* can;
    PacketConverter* converter;
    ChronoModel* chronoModel;
    UniqueModel* uniqueModel;
    StreamingManager* streaming;
    RecordingManager* recording;
    QTimer statsTimer;
    QLabel *ppsLabel, *uvtLabel;

    void closeEvent(QCloseEvent *evt){
        saveTXSettings();
        QMainWindow::closeEvent(evt);
    }

};

#endif // MAINWINDOW_H
