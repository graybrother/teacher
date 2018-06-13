#-------------------------------------------------
#
# Project created by QtCreator 2017-12-11T20:39:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = teacher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    objfeature.cpp \
    vibe-background-sequential.c

HEADERS  += mainwindow.h \
    objfeature.h \
    vibe-background-sequential.h \
    histogram.h \
    colorhistogram.h \
    contentfinder.h

FORMS    += mainwindow.ui
INCLUDEPATH += c:/opencv-contrib/include/opencv \
                 c:/opencv-contrib/include/opencv2 \
                 c:/opencv-contrib/include


LIBS +=  c:/opencv-contrib/lib/libopencv_*.dll.a
#LIBS +=  c:/opencv-contrib/lib/libopencv_imgproc320.dll.a
#LIBS +=  c:/opencv-contrib/lib/libopencv_imgcodec320.dll.a
#LIBS +=  c:/opencv-contrib/lib/libopencv_highgui320.dll.a
#LIBS +=  c:/opencv-contrib/lib/libopencv_videoio320.dll.a
#LIBS +=  c:/opencv-contrib/lib/libopencv_video320.dll.a


