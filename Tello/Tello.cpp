#include "Tello.h"

Tello::Tello(){
    QThread::currentThread()->setObjectName("Tello Main Thread");
    TelloLogger::write2log("##################### Tello Main Thread Started #####################");

    if(TELLO_DEBUG_OUTPUT)
        qDebug() << "Starting" << QThread::currentThread();

    //Init Tello Command Object and bind to Thread
    tello_command = new TelloCommand(QHostAddress(address_str), port_command);
    thread_command = new QThread();
    thread_command->setObjectName("Tello Command Thread");
    tello_command->moveToThread(thread_command);
    connect(thread_command,&QThread::started,tello_command,&TelloCommand::run);

    //Init Tello State Object
    tello_state = new TelloState(QHostAddress(address_str), port_state);
    connect(tello_state,&TelloState::status,tello_command,&TelloCommand::updateStatus);

    //Init Tello Stream Object
    tello_stream = new TelloStream(address_str, port_stream);
    connect(tello_command,&TelloCommand::cameraEnabled,tello_stream,&TelloStream::enableStream);
    connect(tello_command,&TelloCommand::cameraDisable,tello_stream,&TelloStream::disableStream);
}

Tello::~Tello(){
    delete tello_state;
    delete tello_command;
    delete tello_stream;

    delete thread_command;
}

void Tello::start(){
    if(started){
        qDebug() << "Tello Already running";
        return;
    }
    started = true;

    thread_command->start();
    tello_command->running(true);
}

void Tello::stop(){
    if(started == false){
        qDebug() << "Not started";
        return;
    }
    started = false;

    tello_command->running(false);
    thread_command->quit();
    thread_command->wait();

    tello_stream->disableStream();
}
