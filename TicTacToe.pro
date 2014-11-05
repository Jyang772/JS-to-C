

TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cell.cpp \
    tictactoeboard.cpp \
    game.cpp \
    clone.cpp \
    montebot.cpp

HEADERS += \
    javascript.h \
    cell.h \
    tictactoeboard.h \
    move.h \
    game.h \
    clone.h \
    montebot.h

