TEMPLATE = lib
CONFIG += staticlib c++11
CONFIG -= qt
TARGET = vote
HEADERS += libvote.hh
SOURCES += \
           libvote.cc \
           alternatif.cc \
           majoritaire.cc \
           preferentiel.cc

