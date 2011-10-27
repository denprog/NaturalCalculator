#ifndef NATURALCALCULATOR_H
#define NATURALCALCULATOR_H

#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include "FormulaWnd.h"

//#if defined(WIN32) && defined(_DEBUG)
//#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#define new DEBUG_NEW
//#endif

class QVBoxLayout;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

public:
	friend class FormulaWnd;

public:
	virtual void closeEvent(QCloseEvent* event);

public:
	void SetupUi();
	
private slots:
	void OnNew();
	void OnOpen();
	void OnSave();
	void OnUndo();
	void OnRedo();
	void OnSettings();

public:
	FormulaWnd* formulaWnd;
	
private:
	QVBoxLayout* mainLayout;
	Settings settings;

	QToolBar* mainToolbar;
};

#endif // NATURALCALCULATOR_H
