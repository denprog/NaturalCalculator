#include <QtGui>
#include <QFileDialog>
#include <vld.h>
#include "MainWindow.h"
#include "SettingsDialog.h"

/**
 * Constructor.
 * @param [in,out] parent The parent widget.
 * @param flags	The flags.
 */
MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags), settings("naturaleditor.org", "NaturalCalculator")
{
	SetupUi();
}

/**
 * Destructor.
 */
MainWindow::~MainWindow()
{
}

/**
 * The close event.
 * @param [in,out] event The event.
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
	//save the settings
	settings.Save("MainWindow", "state", saveState());
	settings.Save("MainWindow", "geometry", saveGeometry());
}

/**
 * Sets up the user interface.
 */
void MainWindow::SetupUi()
{
	resize(600, 400);

	//set the main toolbar	
	mainToolbar = new QToolBar();
	mainToolbar->setEnabled(true);
	mainToolbar->setIconSize(QSize(16, 16));
	addToolBar(Qt::TopToolBarArea, mainToolbar);

	QAction* action = new QAction(QIcon(":/Resources/images/new.png"), tr("&New"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnNew()));
	mainToolbar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/open.png"), tr("&Open"), this);
	action->setShortcut(QKeySequence::Open);
	connect(action, SIGNAL(triggered()), this, SLOT(OnOpen()));
	mainToolbar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/save.png"), tr("&Save"), this);
	action->setShortcut(QKeySequence::Save);
	connect(action, SIGNAL(triggered()), this, SLOT(OnSave()));
	mainToolbar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/undo.png"), tr("&Undo"), this);
	action->setShortcut(QKeySequence::Undo);
	connect(action, SIGNAL(triggered()), this, SLOT(OnUndo()));
	mainToolbar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/redo.png"), tr("&Redo"), this);
	action->setShortcut(QKeySequence::Redo);
	connect(action, SIGNAL(triggered()), this, SLOT(OnRedo()));
	mainToolbar->addAction(action);
	
	action = new QAction(QIcon(":/Resources/images/settings.png"), tr("Se&ttings"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnSettings()));
	mainToolbar->addAction(action);
	
	setWindowTitle(QApplication::translate("Natural Calculator", "Natural Calculator", 0, QApplication::UnicodeUTF8));
	QMetaObject::connectSlotsByName(this);

	mathToolbar = new QToolBar();
	mathToolbar->setEnabled(true);
	addToolBar(Qt::TopToolBarArea, mathToolbar);
	QTabWidget* tab = new QTabWidget;
	mathToolbar->addWidget(tab);

	//fill the algebra toolbar
	
	QToolBar* algebraToolBar = new QToolBar();
	algebraToolBar->setEnabled(true);
	algebraToolBar->setIconSize(QSize(20, 20));

	action = new QAction(QIcon(":/Resources/images/math/plus.png"), tr("Plus"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnPlus()));
	algebraToolBar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/math/minus.png"), tr("Minus"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnMinus()));
	algebraToolBar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/math/multiply.png"), tr("Multiply"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnMultiply()));
	algebraToolBar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/math/division.png"), tr("Division"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnDivision()));
	algebraToolBar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/math/power.png"), tr("Power"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnPower()));
	algebraToolBar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/math/sqrt.png"), tr("Square root"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnSqrt()));
	algebraToolBar->addAction(action);

	action = new QAction(QIcon(":/Resources/images/math/equality.png"), tr("Equality"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnEquality()));
	algebraToolBar->addAction(action);

	tab->addTab(algebraToolBar, "algebra");
	
	//fill the functions toolbar
	
	QToolBar* functionsToolBar = new QToolBar();
	functionsToolBar->setEnabled(true);
	functionsToolBar->setIconSize(QSize(20, 20));

	action = new QAction(QIcon(":/Resources/images/math/sin.png"), tr("&sinus"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(OnSin()));
	functionsToolBar->addAction(action);

	tab->addTab(functionsToolBar, "functions");
	
	restoreState(settings.Load("MainWindow", "state").toByteArray());
	restoreGeometry(settings.Load("MainWindow", "geometry").toByteArray());
	
	//set the main window
	formulaWnd = new FormulaWnd(this);
	formulaWnd->setObjectName(QString::fromUtf8("formulaWnd"));
	formulaWnd->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	QWidget* c = new QWidget();
	
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->addWidget(formulaWnd);

	c->setLayout(layout);
	setCentralWidget(c);

	formulaWnd->activateWindow();
	formulaWnd->setFocus();
}

/**
 * The new button event.
 */
void MainWindow::OnNew()
{
	formulaWnd->New();
}

/**
 * The open button event.
 */
void MainWindow::OnOpen()
{
	QFileDialog d(this);
	d.setFileMode(QFileDialog::ExistingFile);
	d.setNameFilter(tr("Natural Calulator files (*.ncl)"));
	
	if (d.exec())
		formulaWnd->Open(d.selectedFiles()[0]);
}

/**
 * The save button event.
 */
void MainWindow::OnSave()
{
	QFileDialog d(this);
	d.setAcceptMode(QFileDialog::AcceptSave);
	d.setNameFilter(tr("Natural Calulator files (*.ncl)"));
	d.setDefaultSuffix("ncl");
	
	if (d.exec())
		formulaWnd->Save(d.selectedFiles()[0]);
}

/**
 * The undo button event.
 */
void MainWindow::OnUndo()
{
	formulaWnd->Undo();
}

/**
 * The redo button event.
 */
void MainWindow::OnRedo()
{
	formulaWnd->Redo();
}

/**
 * The settings button event.
 */
void MainWindow::OnSettings()
{
	SettingsDialog d(&settings);
	if (d.exec())
		formulaWnd->UpdateView();
}

/**
 * The plus button event.
 */
void MainWindow::OnPlus()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreatePlusFormulaNode);
}

/**
 * The sin button event.
 */
void MainWindow::OnSin()
{
}

/**
 * The minus button event.
 */
void MainWindow::OnMinus()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateMinusFormulaNode);
}

/**
 * The multiply button event.
 */
void MainWindow::OnMultiply()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateMultiplyFormulaNode);
}

/**
 * The division button event.
 */
void MainWindow::OnDivision()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateDivisionFormulaNode);
}

/**
 * The power button event.
 */
void MainWindow::OnPower()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreatePowerFormulaNode);
}

/**
 * The sqrt button event.
 */
void MainWindow::OnSqrt()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateSquareRootFormulaNode);
}

/**
 * The equality button event.
 */
void MainWindow::OnEquality()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateEquationFormulaNode);
}
