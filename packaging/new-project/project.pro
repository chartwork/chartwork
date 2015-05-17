QT += core gui widgets

TARGET = project
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

CONFIG += c++11

INCLUDEPATH += $$[QT_INSTALL_HEADERS]
LIBS += -L$$[QT_INSTALL_LIBS]

CONFIG(debug, debug|release): LIBS += -lchartworkd
CONFIG(release, debug|release): LIBS += -lchartwork
