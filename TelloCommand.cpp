#include "TelloCommand.h"

TelloCommand::TelloCommand(QHostAddress a, quint16 p): ip(a), port(p){
    qDebug() << this << "Constructed on" << QThread::currentThread();
    isRunning = false;

    socket = new QUdpSocket;

    if(!socket->bind(port)){
        qDebug() << socket->errorString();
        emit alertSignal(TelloAlerts::SOCKET_CONNECTION_FAILED);
    }
    else{
        qDebug() << "Local bind ready on " << socket->localAddress() << ":" << socket->localPort();
        connect(socket, &QUdpSocket::readyRead, this, &TelloCommand::readResponse, Qt::DirectConnection);
        sdk_mode_enabled = false;
    }
}

TelloCommand::~TelloCommand(){
    delete socket;
    qDebug() << this << "Deconstructed on" << QThread::currentThread();
}

void TelloCommand::sendCommand_generic(QByteArray cmd){
    qDebug() << "Sent: " << cmd << "on" << QThread::currentThread();
    lastCommandUsed = cmd;
    socket->writeDatagram(cmd, ip, port);
    generic_command_requested = true;
}

void TelloCommand::sendCommand_SNR(){
    socket->writeDatagram("wifi?", ip, port);
    snr_requested = true;
}

void TelloCommand::sendCommand_waitResponse(QByteArray cmd){
/*
    waitingResponse = true;
    QTimer::singleShot(200, this, &Foo::updateCaption);
    /*
    for(int i=0 ; i < 5 ; i++){
        sendCommand_generic(cmd);
        if(answered){
            emit
            return;
        }
        QThread::sleep(1);
    }
*/
}

void TelloCommand::run(){
    sdk_mode_enabled = false;
    while(isRunning){
        if(sdk_mode_enabled){
            sendCommand_SNR();
        }
        else{
            sendCommand_generic("command");
        }
        QThread::sleep(3);
    }
}

void TelloCommand::readResponse(){
    QHostAddress sender;
    quint16 port;

    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

        if(snr_requested){
            qDebug() << "\"snr\"" << ":" << port << "->" << datagram;
        }
        else{
            //qDebug() << sender.toString() << ":" << port << " -> " << datagram;
            qDebug() << lastCommandUsed << ":" << port << "->" << datagram;
        }


        if(generic_command_requested){
            generic_command_requested = false;
            if(lastCommandUsed == "streamon"){
                emit cameraEnabled();
            }
        }


        if (datagram == "timeout"){
            emit responseSignal(TelloResponse::TIMEOUT, datagram);
        }
        else if (datagram == "error"){
            emit responseSignal(TelloResponse::ERROR, datagram);
        }
        else if (datagram == "ok"){
            if(!sdk_mode_enabled){
                sdk_mode_enabled = true;
            }
            emit responseSignal(TelloResponse::OK, datagram);
        }
        else if (datagram.contains("\r\n")){
            if(snr_requested){
                snr_requested = false;
                emit wifiSnrSignal(datagram.toInt());
            }
            else{
                emit responseSignal(TelloResponse::VALUE, datagram);
            }
        }
        else if (datagram == "out of range"){
            emit responseSignal(TelloResponse::OUT_OF_RANGE, datagram);
        }
        else{
            emit responseSignal(TelloResponse::UNDEFINED, datagram);
        }
    }
}
