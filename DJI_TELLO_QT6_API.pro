QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    Tello/Tello.cpp \
    Tello/TelloCommand.cpp \
    Tello/TelloState.cpp \
    Tello/TelloStream.cpp

HEADERS += \
    mainwindow.h \
    Tello/Tello.h \
    Tello/TelloEnumTypes.h \
    Tello/TelloCommand.h \
    Tello/TelloState.h \
    Tello/TelloStream.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OPENCV_DIR = C:\Users\adoucet\Documents\PROJET_LOCAL\opencv\custom_build_QT6

INCLUDEPATH += $$OPENCV_DIR\include
LIBS += $$OPENCV_DIR\bin\libopencv_calib3d470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_core470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_features2d470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_flann470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_gapi470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_highgui470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_imgcodecs470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_imgproc470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_ml470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_objdetect470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_photo470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_stitching470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_video470.dll
LIBS += $$OPENCV_DIR\bin\libopencv_videoio470.dll
LIBS += $$OPENCV_DIR\bin\opencv_videoio_ffmpeg470_64.dll







