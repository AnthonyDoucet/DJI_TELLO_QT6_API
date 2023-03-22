#ifndef TELLOCOMMAND_H
#define TELLOCOMMAND_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <Tello/TelloEnumTypes.h>
#include <Tello/TelloLogger.h>

#define TELLO_COMMAND_DEBUG_OUTPUT true

class TelloCommand: public QObject
{
    Q_OBJECT

public:
    TelloCommand(QHostAddress a, quint16 p);
    ~TelloCommand();

    void takeoff();
    void land();
    void emergency();

    void setPosition(int, int, int, int);
    void setSpeed(int);

    void sendCommand_generic(QByteArray cmd);
    //void sendCommand_waitResponse(QByteArray cmd);
    bool sdkModeEnabled(){ return sdk_mode_enabled; };
    bool isFlying(){ return flying; };
    QString getLastCommandUsed(){ return lastCommandUsed; };
    int getSNR(){ return snr_value;};

public slots:
    void running(bool r){ isRunning = r; };
    void run();
    void updateStatus(TelloStatus new_status);

private:
    TelloStatus status;
    QUdpSocket *socket;
    QHostAddress ip;
    quint16 port;
    bool isRunning, flying, streamEnabled, sdk_mode_enabled, snr_requested, generic_command_requested, wait_command_requested;
    int snr_value;

    qint64 lastTimeCommandSent, lastTimeResponseReceived;
    QString lastCommandUsed;

    void sendCommand_SNR();

private slots:
    void readResponse();

signals:
    void responseSignal(TelloResponse, QString);
    void cameraEnabled();
    void cameraDisable();

};

#endif // TELLO_COMMANDWORKER_H
