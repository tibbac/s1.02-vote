TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle qt
SOURCES += main.cc
INCLUDEPATH += ../libvote
LIBS *= -L../libvote -lvote

