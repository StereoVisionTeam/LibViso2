#-------------------------------------------------
#
# Project created by QtCreator 2014-01-05T02:11:37
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -msse3
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibViso2
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    core.cpp

HEADERS  += mainwindow.h \
    libviso2/demo.h \
    core.h

FORMS    += mainwindow.ui
LIBS += -lpng

HEADERS += libviso2/demo.h \
           libviso2/filter.h \
           libviso2/matcher.h \
           libviso2/matrix.h \
           libviso2/reconstruction.h \
           libviso2/triangle.h \
           libviso2/viso.h \
           libviso2/viso_mono.h \
           libviso2/viso_stereo.h

SOURCES += libviso2/demo.cpp \
           libviso2/filter.cpp \
           libviso2/matcher.cpp \
           libviso2/matrix.cpp \
           libviso2/reconstruction.cpp \
           libviso2/triangle.cpp \
           libviso2/viso.cpp \
           libviso2/viso_mono.cpp \
           libviso2/viso_stereo.cpp

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib

LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lopencv_nonfree
