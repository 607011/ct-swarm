TEMPLATE = lib

DESTDIR = ../debug

TARGET = pso-core

QMAKE_CXXFLAGS += -fopenmp
LIBS += -lgomp

CONFIG = debug staticlib qt thread

include(pso-core.pri)
