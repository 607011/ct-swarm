HEADERS = \
	3dwidget.h \
	fitnessgraph.h \
	helpviewer.h \
	mainwindow.h \
	populatedialog.h \
	pso.h \
	topographwidget.h \
	zfinder.h

FORMS = \
	populatedialog.ui

SOURCES = \
	3dwidget.cpp \
	fitnessgraph.cpp \
	helpviewer.cpp \
	mainwindow.cpp \
	populatedialog.cpp \
	pso.cpp \
	topographwidget.cpp \
	zfinder.cpp \
	main.cpp

RESOURCES = \
	swarm.qrc

TRANSLATIONS = \
	swarm-de_DE.ts

QMAKE_CXXFLAGS += -fopenmp
LIBS += -lgomp

DESTDIR = ../debug

MOC_DIR = ./GeneratedFiles/debug

OBJECTS_DIR += debug

UI_DIR = ./GeneratedFiles

RCC_DIR = ./GeneratedFiles

QT += opengl

TARGET = swarm

DEPENDPATH += .

INCLUDEPATH += . \
    ./GeneratedFiles \
    ./GeneratedFiles/debug \
    ../pso-core

LIBS += \
    -L../debug \
    -lpso-core

win32:RC_FILE = swarm.rc

target.path = .
source.path = .
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS $$TRANSLATIONS *.pro

INSTALLS += target source
