#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tello = new Tello;

    connect(tello->tello_command,&TelloCommand::alertSignal,this,&MainWindow::processAlertSignal);
    connect(tello->tello_command,&TelloCommand::responseSignal,this,&MainWindow::processResponseSignal);
    connect(tello->tello_command,&TelloCommand::wifiSnrSignal,this,&MainWindow::processSNR);

    connect(tello->tello_state,&TelloState::dataAvailable,this,&MainWindow::updateGUI);
    connect(tello->tello_stream,&TelloStream::newFrame,this,&MainWindow::displayStream);

    tello->start();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    /*
    if(tello->isStarted()){
        tello->stop();
        ui->pushButton->setText("Start");
    }
    else{
        tello->start();
        ui->pushButton->setText("Stop");
    }
    */
}

void MainWindow::updateGUI(){
    ui->label_3->setText("Battery: "+QString::number(tello->tello_state->getBattery()) + "%");
}

void MainWindow::processResponseSignal(TelloResponse, QString str){
    ui->label->setText("Response: " + str);
}

void MainWindow::processAlertSignal(TelloAlerts status){
    switch (status) {
    case TelloAlerts::TELLO_CONNECTION_ESTABLISHED:
        ui->label_status->setText("Connected");
        break;
    case TelloAlerts::TELLO_CONNECTION_WAITING:
        ui->label_status->setText("Waiting...");
        break;
    case TelloAlerts::TELLO_CONNECTION_TIMEOUT:
        ui->label_status->setText("Timeout > 10s");
        break;
    case TelloAlerts::TELLO_CONNECTION_FAILED:
        ui->label_status->setText("Failed");
        break;
    default:
        ui->label_status->setText("Error");
        break;
    }
}

void MainWindow::processSNR(int snr){
    ui->label_2->setText("Wifi SNR: " + QString::number(snr));
}

void MainWindow::displayStream(QPixmap frame){
    ui->label_stream->setPixmap(frame);
}

void MainWindow::on_pushButton_2_clicked(){
    //tello->enableStream();
}
