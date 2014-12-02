#-------------------------------------------------
#
# Project created by QtCreator 2012-04-03T15:02:40
#
#-------------------------------------------------

QT       += core gui
QT       += phonon
CONFIG   += gif

TARGET = PI_Player
TEMPLATE = app

RC_FILE = icon.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    myvideowidget.cpp \
    myplayer.cpp \
    pluginwidget.cpp \
    qseturlevent.cpp \
    settingdialog.cpp \
    qctrlplayerevent.cpp

HEADERS  += mainwindow.h \
    myvideowidget.h \
    myplayer.h \
    pluginwidget.h \
    qseturlevent.h \
    settingdialog.h \
    qctrlplayerevent.h

OTHER_FILES +=

RESOURCES += \
    resource.qrc

FORMS += \
    settingdialog.ui

























