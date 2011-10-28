#include <QtGui>
#include <QFileDialog>
#include <vld.h>
#include "MainWindow.h"
#include "SettingsDialog.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags), settings("naturaleditor.org", "MainWindow")
{
	SetupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	settings.Save("NaturalCalculator", "position", pos());
	settings.Save("NaturalCalculator", "size", size());
}

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

	QVariant p = settings.Load("NaturalCalculator", "position");
	move(p.toPoint());
	p = settings.Load("NaturalCalculator", "size");
	resize(p.toSize());
	
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
}

void MainWindow::OnNew()
{
	formulaWnd->New();
}

void MainWindow::OnOpen()
{
	QFileDialog d(this);
	d.setFileMode(QFileDialog::ExistingFile);
	d.setNameFilter(tr("Natural Calulator files (*.ncl)"));
	
	if (d.exec())
		formulaWnd->Open(d.selectedFiles()[0]);
}

void MainWindow::OnSave()
{
	QFileDialog d(this);
	d.setAcceptMode(QFileDialog::AcceptSave);
	d.setNameFilter(tr("Natural Calulator files (*.ncl)"));
	d.setDefaultSuffix("ncl");
	
	if (d.exec())
		formulaWnd->Save(d.selectedFiles()[0]);
}

void MainWindow::OnUndo()
{
	formulaWnd->Undo();
}

void MainWindow::OnRedo()
{
	formulaWnd->Redo();
}

void MainWindow::OnSettings()
{
	SettingsDialog d(&settings);
	if (d.exec())
		formulaWnd->UpdateView();
}

void MainWindow::OnPlus()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreatePlusFormulaNode);
}

void MainWindow::OnSin()
{
}

void MainWindow::OnMinus()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateMinusFormulaNode);
}

void MainWindow::OnMultiply()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateMultiplyFormulaNode);
}

void MainWindow::OnDivision()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateDivisionFormulaNode);
}

void MainWindow::OnPower()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreatePowerFormulaNode);
}

void MainWindow::OnSqrt()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateSquareRootFormulaNode);
}

void MainWindow::OnEquality()
{
	formulaWnd->InsertNode(&FormulaNode::DoCreateEquationFormulaNode);
}
