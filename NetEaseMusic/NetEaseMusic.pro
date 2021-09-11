QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=$$PWD middleWidget
include(middleWidget/middle_widget.pri)

SOURCES += \
    bottom_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowhandle.cpp \
    music_player.cpp \
    skin_config.cpp \
    top_widget.cpp

HEADERS += \
    bottom_widget.h \
    mainwindow.h \
    mainwindowhandle.h \
    music_player.h \
    skin_config.h \
    top_widget.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc \
    res.qrc

DISTFILES += \
    res/css/black.css \
    res/css/netease-red.css \
    res/css/red.css \
    res/txt/music_file_path.txt


INCLUDEPATH +="./ffmpeg_4_0_1/include"
LIBS += -L$$PWD/ffmpeg_4_0_1/lib -lavutil -lavformat -lavcodec -lavdevice -lavfilter -lpostproc -lswresample -lswscale -lSDL2
