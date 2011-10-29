#ifndef NATURALCALCULATOR_H
#define NATURALCALCULATOR_H

#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include "FormulaWnd.h"

class QVBoxLayout;

/**
 * The main window of the application.
 */
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
	
	void OnPlus();
	void OnMinus();
	void OnMultiply();
	void OnDivision();
	void OnPower();
	void OnSqrt();
	void OnEquality();
	
	void OnSin();

public:
	FormulaWnd* formulaWnd; ///< The formula window
	
private:
	QVBoxLayout* mainLayout;
	Settings settings;

	QToolBar* mainToolbar;
	QToolBar* mathToolbar;
};

#endif // NATURALCALCULATOR_H
