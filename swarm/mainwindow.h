// Copyright (c) 2005-2008 Oliver Lau <ola@ctmagazin.de>
// Heise Zeitschriften Verlag, Hannover, Germany

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QCloseEvent>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QButtonGroup>
#include <QMenu>
#include <QSettings>
#include "populatedialog.h"

class PSO;
class ThreeDWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private: // variables
	QToolBar* drawingTools;
	QAction* newAct;
	QAction* openAct;
	QAction* saveAct;
	QAction* saveAsAct;
	QAction* populateAct;
	QAction* exitAct;
	QAction* view3DAct;
	QAction* aboutAct;
	QAction* helpAct;
	QAction* bearModeAct;
	QAction* buildModeAct;
	QAction* destructModeAct;
	QButtonGroup* toolButtons;
	QMenu* fileMenu;
	QMenu* editMenu;
	QMenu* viewMenu;
	QMenu* helpMenu;
	ThreeDWidget* gl;
	PSO* pso;
	QSettings* settings;
	PopulateDialog* populateDialog;
	bool swarmChanged;
	bool graphChanged;

private: // methods
	bool maybeSave(void);
	void writeSettings(void);

private slots:
	void newFile(void);
	void open(void);
	bool save(void);
	bool saveAs(void);
	void about(void);
	void help(void);
	void populate(void);
	void view3D(bool checked);
	void fitnessChanged(void);
	void enterBearMode(bool);
	void enterBuildMode(bool);
	void enterDestructMode(bool);
	void threeDWidgetClosed(void);

public: // methods
    MainWindow(QWidget* parent = NULL);

protected: // methods
	void closeEvent(QCloseEvent*);
	void showEvent(QShowEvent*);

signals:
  	void numberOfParticlesChanged(int);

};

#endif // __MAINWINDOW_H_
