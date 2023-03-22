#include "TelloCommand.h"

TelloCommand::TelloCommand(QHostAddress a, quint16 p): ip(a), port(p){
    if(TELLO_COMMAND_DEBUG_OUTPUT)
        qDebug() << this << "Constructed on" << QThread::currentThread();
    TelloLogger::write2log("Tello Command Constructed");
    isRunning = false;

    socket = new QUdpSocket;

    if(!socket->bind(port)){
        qDebug() << socket->errorString();
    }
    else{
        if(TELLO_COMMAND_DEBUG_OUTPUT)
            qDebug() << "Local bind ready on " << socket->localAddress() << ":" << socket->localPort();
        TelloLogger::write2log("Local bind ready on " + socket->localAddress().toString() + ":" + QString::number(socket->localPort()));

        connect(socket, &QUdpSocket::readyRead, this, &TelloCommand::readResponse, Qt::DirectConnection);
        sdk_mode_enabled = false;
    }
}

TelloCommand::~TelloCommand(){
    delete socket;
    if(TELLO_COMMAND_DEBUG_OUTPUT)
        qDebug() << this << "Deconstructed on" << QThread::currentThread();
    TelloLogger::write2log("Tello Command Destructed");
}

void TelloCommand::takeoff(){
    if(isRunning && !flying){
        flying = true;
        sendCommand_generic("takeoff");
    }
}

void TelloCommand::land(){
    if(isRunning && flying){
        flying = false;
        sendCommand_generic("land");
    }
}

void TelloCommand::emergency(){
    if(TELLO_COMMAND_DEBUG_OUTPUT)
        qDebug() << "Sent: EMERGENCY on" << QThread::currentThread();
    lastCommandUsed = "emergency";
    socket->writeDatagram("emergency", ip, port);
    lastTimeCommandSent = QDateTime::currentSecsSinceEpoch();
    generic_command_requested = true;
    flying = false;
}

void TelloCommand::setPosition(int a, int b, int c, int d){
    QByteArray cmd = "rc " + QByteArray::number(a) + " " + QByteArray::number(b) + " " + QByteArray::number(c) + " " + QByteArray::number(d);
    sendCommand_generic(cmd);
}

void TelloCommand::setSpeed(int speed){
    if(speed >= 10 && speed <= 100){
        sendCommand_generic("speed " + QByteArray::number(speed));
    }
}

void TelloCommand::sendCommand_generic(QByteArray cmd){
    if(isRunning){
        if(TELLO_COMMAND_DEBUG_OUTPUT)
            qDebug() << "Sent: " << cmd << "on" << QThread::currentThread();
        lastCommandUsed = cmd;
        lastTimeCommandSent = QDateTime::currentSecsSinceEpoch();
        socket->writeDatagram(cmd, ip, port);
        TelloLogger::write2log("Tello Command send \""+ cmd + "\"");
        generic_command_requested = true;
    }
    else{
        TelloLogger::write2log("Tello Command Try to send command but was not running");
    }
}

void TelloCommand::sendCommand_SNR(){
    socket->writeDatagram("wifi?", ip, port);
    TelloLogger::write2log("Tello Command SNR Requested");
    lastTimeCommandSent = QDateTime::currentSecsSinceEpoch();
    snr_requested = true;
}

void TelloCommand::run(){
    sdk_mode_enabled = false;
    streamEnabled = false;
    lastTimeCommandSent = 0;
    lastTimeResponseReceived = 0;
    quint16 responseDelay = 0;

    TelloLogger::write2log("Tello Command While loop start");
    while(isRunning){
        if(sdk_mode_enabled){
            if(!streamEnabled){
                sendCommand_generic("streamon");
                TelloLogger::write2log("Tello Command Waiting Video Stream");
            }
            responseDelay = abs(lastTimeResponseReceived - lastTimeCommandSent);
            TelloLogger::write2log("Tello Command Response delay: "+ QString::number(responseDelay)+"s");
            if(TELLO_COMMAND_DEBUG_OUTPUT)
                qDebug() << "response delay: " << QString::number(responseDelay) << "s";

            if(responseDelay >= 10){
                TelloLogger::write2log("Tello Command Delay > 10s -> Timeout waiting, sdk mode and stream disabled");
                //emit alertSignal(TelloAlerts::TELLO_CONNECTION_TIMEOUT);
                QThread::sleep(5);
                emit cameraDisable();
                sdk_mode_enabled = false;
                streamEnabled = false;
            }
            else if(responseDelay >= 5){
                TelloLogger::write2log("Tello Command Delay > 5s -> No Response");
            }

            sendCommand_SNR();
        }
        else{
            TelloLogger::write2log("Tello Command Waiting connection");
            sendCommand_generic("command");
        }
        QThread::sleep(2);
    }
    TelloLogger::write2log("Tello Command While loop end");
}

void TelloCommand::updateStatus(TelloStatus new_status){
    status = new_status;
}

void TelloCommand::readResponse(){

    while (socket->hasPendingDatagrams())
    {
        QHostAddress sender;
        quint16 port;
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

        if(TELLO_COMMAND_DEBUG_OUTPUT){
            if(snr_requested){
                qDebug() << "\"snr\"" << ":" << port << "->" << datagram;
                TelloLogger::write2log("Tello Command Response from \"snr\" -> " + QString(datagram).trimmed());
            }
            else{
                qDebug() << lastCommandUsed << ":" << port << "->" << datagram;
                TelloLogger::write2log("Tello Command Response from \""+ lastCommandUsed + "\" -> " + QString(datagram).trimmed());
            }
        }


        lastTimeResponseReceived = QDateTime::currentSecsSinceEpoch();
        if(generic_command_requested){
            generic_command_requested = false;
            if(lastCommandUsed == "streamon"){
                streamEnabled = true;
                emit cameraEnabled();
            }
        }


        if(datagram == "timeout"){ emit responseSignal(TelloResponse::TELLO_TIMEOUT, datagram); }
        else if(datagram == "error"){ emit responseSignal(TelloResponse::TELLO_ERROR, datagram); }
        else if(datagram == "ok"){
            if(!sdk_mode_enabled){
                sdk_mode_enabled = true;
            }
            emit responseSignal(TelloResponse::TELLO_OK, datagram);
        }
        else if(datagram.contains("\r\n")){
            if(snr_requested){
                snr_requested = false;
                snr_value = datagram.toInt();
            }
            else{
                emit responseSignal(TelloResponse::TELLO_VALUE, datagram);
            }
        }
        else if(datagram == "out of range"){ emit responseSignal(TelloResponse::TELLO_OUT_OF_RANGE, datagram); }
        else{ emit responseSignal(TelloResponse::TELLO_UNDEFINED, datagram); }
    }
}
