#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tello = new Tello;

    connect(tello->tello_command,&TelloCommand::alertSignal,this,&MainWindow::processAlertSignal);
    connect(tello->tello_command,&TelloCommand::responseSignal,this,&MainWindow::processResponseSignal);
    connect(tello->tello_state,&TelloState::dataAvailable,this,&MainWindow::updateGUI);
    connect(tello->tello_stream,&TelloStream::newFrame,this,&MainWindow::displayStream);

    tello->start();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::updateGUI(){
    ui->label_battery->setText("Battery: "+QString::number(tello->tello_state->getBattery()) + "%");
    ui->label_snr->setText("SNR: "+QString::number(tello->tello_command->getSNR()));

    QString Pitch = "Pitch: " + QString::number(tello->tello_state->getPitch()) + "°";
    QString Roll = "Roll: " + QString::number(tello->tello_state->getRoll()) + "°";
    QString Yaw = "Yaw: " + QString::number(tello->tello_state->getYaw()) + "°";
    ui->label_pitch_roll_yaw->setText(Pitch + "  " + Roll + "  " + Yaw);
    ui->label_height->setText("Height: "+QString::number(tello->tello_state->getHeight()) + "cm");
}

void MainWindow::processResponseSignal(TelloResponse, QString str){
    ui->lineEdit_response->setText(tello->tello_command->getLastCommandUsed() + " -> " + str);
}

void MainWindow::processAlertSignal(TelloAlerts status){
    QPixmap img;

    switch (status) {
    case TelloAlerts::TELLO_CONNECTION_ESTABLISHED:
        ui->label_status->setText("Connected");
        break;
    case TelloAlerts::TELLO_CONNECTION_WAITING:
        ui->label_status->setText("Waiting...");
        break;
    case TelloAlerts::TELLO_CONNECTION_NO_RESPONSE:
        ui->label_status->setText("No Response");
        break;
    case TelloAlerts::TELLO_CONNECTION_TIMEOUT:
        ui->label_status->setText("No Response since 10s -> reset connection");
        img.fill(Qt::white);
        displayStream(img);
        break;
    case TelloAlerts::TELLO_CONNECTION_FAILED:
        ui->label_status->setText("Failed");
        break;
    default:
        ui->label_status->setText("Error");
        break;
    }
}

void MainWindow::displayStream(QPixmap frame){
    ui->label_stream->setPixmap(frame);
}

void MainWindow::on_pushButton_clicked(){
    tello->tello_command->sendCommand_generic(ui->lineEdit_send->text().toUtf8());
}


void MainWindow::on_btn_takeoff_clicked(){
    tello->tello_command->takeoff();
}

void MainWindow::on_btn_land_clicked(){
    tello->tello_command->land();
}

void MainWindow::on_btn_emergency_clicked(){
    tello->tello_command->emergency();
}

void MainWindow::on_btn_test_clicked(){

}

