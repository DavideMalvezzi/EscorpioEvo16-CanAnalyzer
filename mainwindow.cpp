#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hexvalidator.h"
#include "hexconverterdialog.h"
#include "filter/createfilterdialog.h"

#include <QFileDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QSignalMapper>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //Load serialports
    portCombo = new QComboBox;
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions().first(), portCombo);
    on_actionRefresh_triggered();

    //Load settings
    loadSerialPortParams();
    on_actionSettings_triggered();
    ui->stackedWidget->setCurrentIndex(0);

    //Converter
    can = new CanInterface;
    can->start();
    converter = new PacketConverter(ui->dbPathText->text());
    connect(can, SIGNAL(packetReceived(unsigned short,QByteArray)),
            converter, SLOT(convertPacket(unsigned short,QByteArray)),
            Qt::QueuedConnection);
    connect(converter, SIGNAL(packetConverted(ConversionResponse*)),
            this, SLOT(onDataReady(ConversionResponse*)));

    //RX
    chronoModel = new ChronoModel(this);
    ResponseFilterModel* cfilterModel = new ResponseFilterModel(this);
    cfilterModel->setSourceModel(chronoModel);
    cfilterModel->setDynamicSortFilter(true);
    ui->rxChronoTable->setModel(cfilterModel);
    ui->rxChronoTable->setItemDelegate(new PlainStyleDelegate(ui->rxChronoTable));

    uniqueModel = new UniqueModel(this);
    ResponseFilterModel* ufilterModel = new ResponseFilterModel(this);
    ufilterModel->setSourceModel(uniqueModel);
    ufilterModel->setDynamicSortFilter(true);
    ui->rxUniqueTable->setModel(ufilterModel);
    ui->rxUniqueTable->setItemDelegate(new PlainStyleDelegate(ui->rxUniqueTable));


    int c;
    for (c = 0; c < ui->rxChronoTable->horizontalHeader()->count() - 1; c++){
        ui->rxChronoTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    }
    ui->rxChronoTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);

    for (c = 0; c < ui->rxUniqueTable->horizontalHeader()->count() - 1; c++){
        ui->rxUniqueTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    }
    ui->rxUniqueTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);

    //ui->rxChronoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->rxUniqueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //TX
    loadTXParams();
    loadTXSettings();

    //Streaming
    streamEnabled = false;
    streaming = new StreamingManager; //no parent cause moveToThread
    streaming->start();

    //Recording
    recordEnabled = false;
    recording = new RecordingManager; //no parente cause moveToThread
    recording->start();

    //Actions
    ui->actionPause->setVisible(false);
    ui->actionEndStreaming->setVisible(false);

    //Stats
    ppsLabel = new QLabel(this);
    uvtLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(ppsLabel);
    ui->statusBar->addPermanentWidget(uvtLabel);
    statsTimer.setInterval(1000);
    statsTimer.start();
    connect(&statsTimer, SIGNAL(timeout()), this, SLOT(showStats()));

    //qDebug()<<"main "<<QThread::currentThreadId();
}

MainWindow::~MainWindow(){
    delete ui;
}

//Config stuff
void MainWindow::loadSerialPortParams(){
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);

    ui->parityBox->addItem("None", QSerialPort::NoParity);
    ui->parityBox->addItem("Even", QSerialPort::EvenParity);
    ui->parityBox->addItem("Odd", QSerialPort::OddParity);
    ui->parityBox->addItem("Mark", QSerialPort::MarkParity);
    ui->parityBox->addItem("Space", QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->stopBitsBox->addItem("1.5", QSerialPort::OneAndHalfStop);
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem("None", QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem("RTS/CTS", QSerialPort::HardwareControl);
    ui->flowControlBox->addItem("XON/XOFF", QSerialPort::SoftwareControl);
}

void MainWindow::on_actionSettings_triggered(){
    QSettings settings("CANAnalyzer.ini", QSettings::IniFormat);

    settings.beginGroup("Serialport");
        ui->baudRateBox->setCurrentIndex(settings.value("baud", 0).toInt());
        ui->stopBitsBox->setCurrentIndex(settings.value("stopbits", 0).toInt());
        ui->dataBitsBox->setCurrentIndex(settings.value("databits", 3).toInt());
        ui->parityBox->setCurrentIndex(settings.value("parity", 0).toInt());
        ui->flowControlBox->setCurrentIndex(settings.value("flowcontrol", 0).toInt());
    settings.endGroup();

    settings.beginGroup("Database");
        ui->dbPathText->setText(settings.value("path", "").toString());
    settings.endGroup();

    settings.beginGroup("Network");
        ui->udpPortSpin->setValue(settings.value("udpport", 5678).toInt());
    settings.endGroup();

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_cancelButton_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_okButton_clicked(){
    QSettings settings("CANAnalyzer.ini", QSettings::IniFormat);
    settings.beginGroup("Serialport");
        settings.setValue("baud", ui->baudRateBox->currentIndex());
        settings.setValue("databits", ui->dataBitsBox->currentIndex());
        settings.setValue("parity", ui->parityBox->currentIndex());
        settings.setValue("stopbits", ui->stopBitsBox->currentIndex());
        settings.setValue("flowcontrol", ui->flowControlBox->currentIndex());
    settings.endGroup();

    settings.beginGroup("Database");
        settings.setValue("path", ui->dbPathText->text());
    settings.endGroup();

    settings.beginGroup("Network");
        settings.setValue("udpport", ui->udpPortSpin->value());
    settings.endGroup();

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_dbPathButton_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this, "Select DB", "", "SQLite DB (*.db *.sql *.sqlite)");
    if(!fileName.isNull() && !fileName.isEmpty()){
        ui->dbPathText->setText(fileName);
    }
}

//Actions
void MainWindow::on_actionRefresh_triggered(){
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    portCombo->clear();
    for(int i=0; i<ports.size(); i++){
        portCombo->addItem(ports.at(i).portName());
    }
}

void MainWindow::on_actionPlay_triggered(){
    if(portCombo->currentIndex() != -1){
        QSerialPort* port = can->getPort();
        port->setPortName(portCombo->currentText());
        port->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->baudRateBox->currentData().toInt()));
        port->setDataBits(static_cast<QSerialPort::DataBits>(ui->dataBitsBox->currentData().toInt()));
        port->setParity(static_cast<QSerialPort::Parity>(ui->parityBox->currentData().toInt()));
        port->setStopBits(static_cast<QSerialPort::StopBits>(ui->stopBitsBox->currentData().toInt()));
        port->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->flowControlBox->currentData().toInt()));
        if(can->play()){
            ui->actionPlay->setVisible(false);
            ui->actionPause->setVisible(true);
            ui->actionStop->setEnabled(true);
            ui->actionRecord->setEnabled(!recordEnabled);
            ui->statusBar->showMessage("Serial port opened");
        }else{
            ui->statusBar->showMessage("Serial port error");
        }
    }else{
        QMessageBox::critical(this, "Error", "No serial port selected");
    }
}

void MainWindow::on_actionPause_triggered(){
    can->pause();
    ui->actionPlay->setVisible(true);
    ui->actionPause->setVisible(false);

    ui->statusBar->showMessage("Serial port paused");

}

void MainWindow::on_actionStop_triggered(){
    can->stop();
    ui->actionPlay->setVisible(true);
    ui->actionPause->setVisible(false);
    ui->actionStop->setEnabled(false);
    ui->actionRecord->setEnabled(false);
    ui->statusBar->showMessage("Serial port closed");

    if(recordEnabled){
        recording->stopRecording();
        recordEnabled = false;
    }
}

void MainWindow::on_actionRecord_triggered(){
    recordEnabled = recording->startRecording(ui->dbPathText->text());
    ui->actionRecord->setEnabled(!recordEnabled);
}

void MainWindow::on_actionFilter_triggered(){
    ResponseFilterModel* cmodel = (ResponseFilterModel*)ui->rxChronoTable->model();
    ResponseFilterModel* umodel = (ResponseFilterModel*)ui->rxUniqueTable->model();

    CreateFilterDialog d(cmodel->getFilter(), converter->getChannelsProps().values());
    if(d.exec()){
        cmodel->setFilter(d.getFilter());
        umodel->setFilter(d.getFilter());
    }
}

void MainWindow::on_actionChronology_triggered(){
    ui->rxView->setCurrentIndex(1);
    ui->actionUnique->setChecked(false);
    ui->tabView->setTabText(0, "RX (Chrono)");
}

void MainWindow::on_actionUnique_triggered(){
    ui->rxView->setCurrentIndex(0);
    ui->actionChronology->setChecked(false);
    ui->tabView->setTabText(0, "RX (Unique)");
}

void MainWindow::on_actionClear_triggered(){
    chronoModel->clear();
    uniqueModel->clear();
    while( ui->txTable->rowCount() > 0){
         ui->txTable->removeRow(0);
    }
}

void MainWindow::on_actionExport_triggered(){
   QString file = QFileDialog::getSaveFileName(this, "Select file name", "", "Excel file (*.xml)");
   if(!file.isEmpty() && !file.isNull()){
       ((ResponseFilterModel*)ui->rxChronoTable->model())->exportToFile(file);
   }
}

//RX
void MainWindow::onDataReady(ConversionResponse* response){

    chronoModel->addResponse(response);
    uniqueModel->addResponse(response);

    if(ui->actionScrollDown->isChecked()){
       ui->rxChronoTable->selectRow(ui->rxChronoTable->model()->rowCount() - 1);
    }
/*
    ui->rxUniqueTable->resizeColumnsToContents();
    ui->rxChronoTable->resizeColumnsToContents();
    ui->rxUniqueTable->horizontalHeader()->setStretchLastSection(true);
    ui->rxChronoTable->horizontalHeader()->setStretchLastSection(true);
*/
   if(response->channel != NULL){
        if(streamEnabled){
            sendOverNetwork(response);
        }

        if(recordEnabled){
            emit recording->addToSaveQueue(response->id, response->result);
        }
    }


}

//TX
void MainWindow::setKnownChannelFromCombo(QWidget* dest){
    QSpinBox* s = (QSpinBox*)dest;
    QComboBox* c = (QComboBox*)(((QSignalMapper*)QObject::sender())->mapping(dest));
    s->setValue(c->currentData().toInt());
}

void MainWindow::setKnownChannelFromSpin(QWidget* dest){
    QSpinBox* s = (QSpinBox*)(((QSignalMapper*)QObject::sender())->mapping(dest));
    QComboBox* c = (QComboBox*)dest;
    c->setCurrentIndex(c->findData(s->value()));
}

void MainWindow::sendData(unsigned int id, QString data){
    if(data.size() == 0){
        QMessageBox::critical(this, "Error", "Sending error: data is null");
        return;
    }

    if(data.size() % 2 == 1){
        QMessageBox::critical(this, "Error", "Sending error: data size is odd");
        return;
    }

    Channel* c = converter->getChannelProp(id);
    if(c != NULL){
        if(data.size()/2 != c->size){
            int r = QMessageBox::warning(this, "Warning", "Sending warning: data size to send is not equal to the size saved in the database. Do you want still send it?", QMessageBox::Yes, QMessageBox::No);
            if(r != QMessageBox::Yes ) return;
        }
    }

    QByteArray bytesData;
    QString byte;
    for(int i = 0; i <data.size(); i += 2){
        byte.clear();
        byte += data.at(i);
        byte += data.at(i + 1);

        bytesData.append((unsigned char)byte.toUInt(NULL, 16));
    }

    if(can->sendMsg(id, bytesData)){
        int row = ui->txTable->rowCount();
        QAbstractItemModel* model = ui->txTable->model();
        ui->txTable->insertRow(row);

        model->setData(model->index(row, 0), QString("0x%1").arg(QString::number(id, 16)));
        model->setData(model->index(row, 1), c == NULL ? "UNKNOWN" : c->name);
        model->setData(model->index(row, 2), bytesData.size());
        for(int i = 0; i < bytesData.size(); i++){
            byte = QString::number((unsigned char)bytesData.at(i), 16).toUpper();
            model->setData(model->index(row, 3 + i), byte.size() < 2 ? byte.prepend('0') : byte);
        }
    }
}

void MainWindow::loadTXParams(){
    QList<Channel*> channels = converter->getChannelsProps().values();
    for(int i = 0; i < channels.size(); i++){
        ui->sendIDCombo_20->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_21->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_22->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_23->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_24->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_25->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_26->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_27->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_28->addItem(channels.at(i)->name, channels.at(i)->id);
        ui->sendIDCombo_29->addItem(channels.at(i)->name, channels.at(i)->id);
    }

    ui->sendIDCombo_20->setCurrentIndex(-1);
    ui->sendIDCombo_21->setCurrentIndex(-1);
    ui->sendIDCombo_22->setCurrentIndex(-1);
    ui->sendIDCombo_23->setCurrentIndex(-1);
    ui->sendIDCombo_24->setCurrentIndex(-1);
    ui->sendIDCombo_25->setCurrentIndex(-1);
    ui->sendIDCombo_26->setCurrentIndex(-1);
    ui->sendIDCombo_27->setCurrentIndex(-1);
    ui->sendIDCombo_28->setCurrentIndex(-1);
    ui->sendIDCombo_29->setCurrentIndex(-1);

    QSignalMapper* comboMapper = new QSignalMapper(this);
    comboMapper->setMapping(ui->sendIDCombo_20, ui->sendIDSpin_20);
    comboMapper->setMapping(ui->sendIDCombo_21, ui->sendIDSpin_21);
    comboMapper->setMapping(ui->sendIDCombo_22, ui->sendIDSpin_22);
    comboMapper->setMapping(ui->sendIDCombo_23, ui->sendIDSpin_23);
    comboMapper->setMapping(ui->sendIDCombo_24, ui->sendIDSpin_24);
    comboMapper->setMapping(ui->sendIDCombo_25, ui->sendIDSpin_25);
    comboMapper->setMapping(ui->sendIDCombo_26, ui->sendIDSpin_26);
    comboMapper->setMapping(ui->sendIDCombo_27, ui->sendIDSpin_27);
    comboMapper->setMapping(ui->sendIDCombo_28, ui->sendIDSpin_28);
    comboMapper->setMapping(ui->sendIDCombo_29, ui->sendIDSpin_29);

    connect(ui->sendIDCombo_20, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_21, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_22, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_23, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_24, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_25, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_26, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_27, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_28, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(ui->sendIDCombo_29, SIGNAL(activated(int)), comboMapper, SLOT(map()));
    connect(comboMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setKnownChannelFromCombo(QWidget*)));

    QSignalMapper* spinMapper = new QSignalMapper(this);
    spinMapper->setMapping(ui->sendIDSpin_20, ui->sendIDCombo_20);
    spinMapper->setMapping(ui->sendIDSpin_21, ui->sendIDCombo_21);
    spinMapper->setMapping(ui->sendIDSpin_22, ui->sendIDCombo_22);
    spinMapper->setMapping(ui->sendIDSpin_23, ui->sendIDCombo_23);
    spinMapper->setMapping(ui->sendIDSpin_24, ui->sendIDCombo_24);
    spinMapper->setMapping(ui->sendIDSpin_25, ui->sendIDCombo_25);
    spinMapper->setMapping(ui->sendIDSpin_26, ui->sendIDCombo_26);
    spinMapper->setMapping(ui->sendIDSpin_27, ui->sendIDCombo_27);
    spinMapper->setMapping(ui->sendIDSpin_28, ui->sendIDCombo_28);
    spinMapper->setMapping(ui->sendIDSpin_29, ui->sendIDCombo_29);

    connect(ui->sendIDSpin_20, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_21, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_22, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_23, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_24, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_25, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_26, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_27, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_28, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(ui->sendIDSpin_29, SIGNAL(valueChanged(int)), spinMapper, SLOT(map()));
    connect(spinMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setKnownChannelFromSpin(QWidget*)));

    HexValidator* hexValidator = new HexValidator(this);
    ui->sendDataText_20->setValidator(hexValidator);
    ui->sendDataText_21->setValidator(hexValidator);
    ui->sendDataText_22->setValidator(hexValidator);
    ui->sendDataText_23->setValidator(hexValidator);
    ui->sendDataText_24->setValidator(hexValidator);
    ui->sendDataText_25->setValidator(hexValidator);
    ui->sendDataText_26->setValidator(hexValidator);
    ui->sendDataText_27->setValidator(hexValidator);
    ui->sendDataText_28->setValidator(hexValidator);
    ui->sendDataText_29->setValidator(hexValidator);

    ui->txTable->setItemDelegate(new PlainStyleDelegate(ui->txTable));
    ui->txTable->resizeColumnsToContents();
    ui->txTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::loadTXSettings(){
    QSettings settings("CANAnalyzer.ini", QSettings::IniFormat);
    settings.beginGroup("TX");
        ui->sendIDSpin_20->setValue(settings.value("id0", 0).toInt());
        ui->sendIDSpin_21->setValue(settings.value("id1", 0).toInt());
        ui->sendIDSpin_22->setValue(settings.value("id2", 0).toInt());
        ui->sendIDSpin_23->setValue(settings.value("id3", 0).toInt());
        ui->sendIDSpin_24->setValue(settings.value("id4", 0).toInt());
        ui->sendIDSpin_25->setValue(settings.value("id5", 0).toInt());
        ui->sendIDSpin_26->setValue(settings.value("id6", 0).toInt());
        ui->sendIDSpin_27->setValue(settings.value("id7", 0).toInt());
        ui->sendIDSpin_28->setValue(settings.value("id8", 0).toInt());
        ui->sendIDSpin_29->setValue(settings.value("id9", 0).toInt());

        ui->sendDataText_20->setText(settings.value("value0", "").toString());
        ui->sendDataText_21->setText(settings.value("value1", "").toString());
        ui->sendDataText_22->setText(settings.value("value2", "").toString());
        ui->sendDataText_23->setText(settings.value("value3", "").toString());
        ui->sendDataText_24->setText(settings.value("value4", "").toString());
        ui->sendDataText_25->setText(settings.value("value5", "").toString());
        ui->sendDataText_26->setText(settings.value("value6", "").toString());
        ui->sendDataText_27->setText(settings.value("value7", "").toString());
        ui->sendDataText_28->setText(settings.value("value8", "").toString());
        ui->sendDataText_29->setText(settings.value("value9", "").toString());
    settings.endGroup();

}

void MainWindow::saveTXSettings(){
    QSettings settings("CANAnalyzer.ini", QSettings::IniFormat);
    settings.beginGroup("TX");
        settings.setValue("id0", ui->sendIDSpin_20->value());
        settings.setValue("id1", ui->sendIDSpin_21->value());
        settings.setValue("id2", ui->sendIDSpin_22->value());
        settings.setValue("id3", ui->sendIDSpin_23->value());
        settings.setValue("id4", ui->sendIDSpin_24->value());
        settings.setValue("id5", ui->sendIDSpin_25->value());
        settings.setValue("id6", ui->sendIDSpin_26->value());
        settings.setValue("id7", ui->sendIDSpin_27->value());
        settings.setValue("id8", ui->sendIDSpin_28->value());
        settings.setValue("id9", ui->sendIDSpin_29->value());

        settings.setValue("value0", ui->sendDataText_20->text());
        settings.setValue("value1", ui->sendDataText_21->text());
        settings.setValue("value2", ui->sendDataText_22->text());
        settings.setValue("value3", ui->sendDataText_23->text());
        settings.setValue("value4", ui->sendDataText_24->text());
        settings.setValue("value5", ui->sendDataText_25->text());
        settings.setValue("value6", ui->sendDataText_26->text());
        settings.setValue("value7", ui->sendDataText_27->text());
        settings.setValue("value8", ui->sendDataText_28->text());
        settings.setValue("value9", ui->sendDataText_29->text());
    settings.endGroup();
}

void MainWindow::on_sendDataButton_20_clicked(){
    sendData(ui->sendIDSpin_20->value(), ui->sendDataText_20->text());
}

void MainWindow::on_sendDataButton_21_clicked(){
    sendData(ui->sendIDSpin_21->value(), ui->sendDataText_21->text());
}

void MainWindow::on_sendDataButton_22_clicked(){
    sendData(ui->sendIDSpin_22->value(), ui->sendDataText_22->text());
}

void MainWindow::on_sendDataButton_23_clicked(){
    sendData(ui->sendIDSpin_23->value(), ui->sendDataText_23->text());
}

void MainWindow::on_sendDataButton_24_clicked(){
    sendData(ui->sendIDSpin_24->value(), ui->sendDataText_24->text());
}

void MainWindow::on_sendDataButton_25_clicked(){
    sendData(ui->sendIDSpin_25->value(), ui->sendDataText_25->text());
}

void MainWindow::on_sendDataButton_26_clicked(){
    sendData(ui->sendIDSpin_26->value(), ui->sendDataText_26->text());
}

void MainWindow::on_sendDataButton_27_clicked(){
    sendData(ui->sendIDSpin_27->value(), ui->sendDataText_27->text());
}

void MainWindow::on_sendDataButton_28_clicked(){
    sendData(ui->sendIDSpin_28->value(), ui->sendDataText_28->text());
}

void MainWindow::on_sendDataButton_29_clicked(){
    sendData(ui->sendIDSpin_29->value(), ui->sendDataText_29->text());
}


//Streaming
void MainWindow::sendOverNetwork(ConversionResponse* r){
    QByteArray datagram;
    if(r->channel != NULL){
        datagram.append(r->id >> 8).append(r->id).append(r->result);
        emit streaming->addToStreamQueue(datagram);
    }
}

void MainWindow::on_actionStartStreaming_triggered(){
    streamEnabled = true;
    streaming->setPort(ui->udpPortSpin->value());
    ui->actionEndStreaming->setVisible(true);
    ui->actionStartStreaming->setVisible(false);
    ui->statusBar->showMessage("Network streaming enabled");
}

void MainWindow::on_actionEndStreaming_triggered(){
    streamEnabled = false;
    ui->actionEndStreaming->setVisible(false);
    ui->actionStartStreaming->setVisible(true);
    ui->statusBar->showMessage("Network streaming disabled");
}

//Stats
void MainWindow::showStats(){
    ppsLabel->setText(QString::number(can->getPPS()).append(" Packets/s"));
    uvtLabel->setText(QString::number(converter->getUsedThread())
                     .append("/").append(QString::number(converter->getTotalThread()).append(" Used/Tot")));
}




void MainWindow::on_actionHexConverter_triggered(){
    HexConverterDialog d;
    d.exec();
}

