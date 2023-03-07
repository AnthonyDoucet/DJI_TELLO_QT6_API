#  Tello API


# Tello class
```c++
//Methods:
start()
stop()
enableStream()
disableStream()
isStarted()
//Access pointer
TelloCommand *tello_command;
TelloState *tello_state;
TelloStream *tello_stream;
```
### Example:
```c++
#include <Tello.h>

//Tello init in MainWindow
Tello *tello;
tello = new Tello;

connect(tello->tello_command,&TelloCommand::responseSignal,this,&MainWindow::processResponseSignal);
connect(tello->tello_state,&TelloState::dataAvailable,this,&MainWindow::updateGUI);
connect(tello->tello_stream,&TelloStream::newFrame,this,&MainWindow::displayStream);

//Tello control
tello->start();

if(tello->isStarted && tello->tello_command->sdk_mode_enabled){
    tello->enableStream();

    tello->tello_command->takeoff();

    int battery = tello->tello_state->getBattery()
}
```

# Tello Command class
```c++
//Methods
void takeoff()
void land()
void emergency()
void setPosition(left/right, forward/backward, up/down, yaw) //value from -100 to 100
void setSpeed(int speed)
void sendCommand_generic(QByteArray cmd)
bool sdkModeEnabled()
bool isFlying()
QString getLastCommandUsed()

//Signals
alertSignal(TelloAlerts)
responseSignal(TelloResponse, QString)
cameraEnabled()
```

# Tello State class
```c++
//Methods
int getMid()
int getX()
int getY()
int getZ()
int getPitch()
int getRoll()
int getYaw()
int getTempLowest()
int getTempHighest()
int getDistanceTof()
int getHeight()
int getBattery()

double *getMpry()
double getVgx()
double getVgy()
double getVgz()
double getBaro()
double getTime()
double getAgx()
double getAgy()
double getAgz()
```


# Tello Stream class
```c++
void run();
bool isCameraEnabled()
QSize getCaptureSize()
void stopStream()
static QPixmap mat2pixmap(Mat)
QSize size2qsize(cv::Size)
```