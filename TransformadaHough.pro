TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    houghtransform.cpp \
    common.cpp \
    houghtransformcircle.cpp

HEADERS += \
    houghtransform.h \
    common.h \
    houghtransformcircle.h

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc
