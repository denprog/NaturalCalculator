#include <QtGui>
#include <vld.h>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags), settings("naturaleditor.org", "MainWindow")
{
	SetupUi();
}

MainWindow::~MainWindow()
{
	//delete newAction;
	//mainToolbar->removeAction(newAction);
	//delete mainToolbar;
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

	newAction = new QAction(QIcon(":/Resources/images/new.png"), tr("&New"), this);
	connect(newAction, SIGNAL(triggered()), this, SLOT(OnNew()));
	mainToolbar->addAction(newAction);

	openAction = new QAction(QIcon(":/Resources/images/open.png"), tr("&Open"), this);
	openAction->setShortcut(QKeySequence::Open);
	connect(openAction, SIGNAL(triggered()), this, SLOT(OnOpen()));
	mainToolbar->addAction(openAction);

	saveAction = new QAction(QIcon(":/Resources/images/save.png"), tr("&Save"), this);
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(OnSave()));
	mainToolbar->addAction(saveAction);

	undoAction = new QAction(QIcon(":/Resources/images/undo.png"), tr("&Undo"), this);
	undoAction->setShortcut(QKeySequence::Undo);
	connect(undoAction, SIGNAL(triggered()), this, SLOT(OnUndo()));
	mainToolbar->addAction(undoAction);

	redoAction = new QAction(QIcon(":/Resources/images/redo.png"), tr("&Redo"), this);
	redoAction->setShortcut(QKeySequence::Redo);
	connect(redoAction, SIGNAL(triggered()), this, SLOT(OnRedo()));
	mainToolbar->addAction(redoAction);

	setWindowTitle(QApplication::translate("Natural Calculator", "Natural Calculator", 0, QApplication::UnicodeUTF8));
	QMetaObject::connectSlotsByName(this);

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
}

void MainWindow::OnNew()
{
}

void MainWindow::OnOpen()
{
	formulaWnd->Open("sample.ncl");
}

void MainWindow::OnSave()
{
	formulaWnd->Save("sample.ncl");
}

void MainWindow::OnUndo()
{
	formulaWnd->Undo();
}

void MainWindow::OnRedo()
{
	formulaWnd->Redo();
}
