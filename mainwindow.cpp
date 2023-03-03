#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tello = new Tello;

    connect(tello->tello_command,&TelloCommand::responseSignal,this,&MainWindow::processResponseSignal);
    connect(tello->tello_command,&TelloCommand::wifiSnrSignal,this,&MainWindow::processSNR);

    connect(tello->tello_state,&TelloState::dataAvailable,this,&MainWindow::updateGUI);
    connect(tello->tello_stream,&TelloStream::newFrame,this,&MainWindow::displayStream);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    if(tello->isStarted()){
        tello->stop();
        ui->pushButton->setText("Start");
    }
    else{
        tello->start();
        ui->pushButton->setText("Stop");
    }
}

void MainWindow::updateGUI(){
    ui->label_3->setText("Battery: "+QString::number(tello->tello_state->getBattery()) + "%");
}

void MainWindow::processResponseSignal(TelloResponse, QString str){
    ui->label->setText("Response: " + str);
}

void MainWindow::processSNR(int snr){
    ui->label_2->setText("Wifi SNR: " + QString::number(snr));
}

void MainWindow::displayStream(QPixmap frame){
    ui->label_stream->setPixmap(frame);
}

void MainWindow::on_pushButton_2_clicked(){
    tello->enableCamera();
}

void MainWindow::on_pushButton_3_clicked(){
    tello->disableCamera();
}

