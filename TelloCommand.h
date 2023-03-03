#ifndef TELLOCOMMAND_H
#define TELLOCOMMAND_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <TelloEnumTypes.h>

class TelloCommand: public QObject
{
    Q_OBJECT

public:
    TelloCommand(QHostAddress a, quint16 p);
    ~TelloCommand();
    bool sdkModeEnabled(){ return sdk_mode_enabled; };
    void sendCommand_generic(QByteArray cmd);
    void sendCommand_waitResponse(QByteArray cmd);

public slots:
    void running(bool r){ isRunning = r; };
    void run();

private:
    QUdpSocket *socket;
    QHostAddress ip;
    quint16 port;
    bool isRunning, sdk_mode_enabled, snr_requested, generic_command_requested;
    int timeout_counter;
    QString lastCommandUsed;
    void sendCommand_SNR();

private slots:
    void readResponse();

signals:
    void alertSignal(TelloAlerts);
    void responseSignal(TelloResponse, QString);
    void wifiSnrSignal(int);
    void cameraEnabled();

};

#endif // TELLO_COMMANDWORKER_H
