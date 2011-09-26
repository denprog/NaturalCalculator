#include "formulawnd.h"
#include <QApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QScrollBar>
#include "MainWindow.h"
#include "../FormulaNodes/DocumentFormulaNode.h"
#include "../FormulaNodes/TextFormulaNode.h"
#include "../FormulaNodes/PlusFormulaNode.h"
#include "../FormulaNodes/SquareRootFormulaNode.h"
#include "../FormulaNodes/MultiplyFormulaNode.h"
#include "../FormulaNodes/MinusFormulaNode.h"
#include "../FormulaNodes/DivisionFormulaNode.h"

FormulaWnd::FormulaWnd(QWidget *parent)	: QGraphicsView(parent), commandManager(this)
{
	scene = new QGraphicsScene();
	setScene(scene);

	setMinimumSize(200, 200);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
		
	settings = &((MainWindow*)parent)->settings;

	caret = new Caret(this);
	
	documentNode = new DocumentFormulaNode(this);
	documentNode->AddLine();
	documentNode->Remake();

	caret->SetToNodeBegin(documentNode);
	caret->Render();
}

FormulaWnd::~FormulaWnd()
{
	delete caret;
	delete documentNode;
	delete scene;
}

void FormulaWnd::resizeEvent(QResizeEvent* event)
{
	QGraphicsView::resizeEvent(event);
	//scene->setSceneRect(rect());
	//scene->setSceneRect(0, 0, 200, 600);
	//scene->setSceneRect(documentNode->boundingRect);
	
	//QRectF r1 = rect(), r2 = documentNode->boundingRect;
	//QRectF r(0, 0, max(r1.width() - 2, r2.width()), max(r1.height() - 2, r2.height()));

	//QScrollBar* h = horizontalScrollBar();
	//if (h->isVisible())
	//{
	//	r.setHeight(r.height() - h->height());
	//}

	//QScrollBar* v = verticalScrollBar();
	//if (v->isVisible())
	//{
	//	r.setWidth(r.width() - v->width());
	//}

	//scene->setSceneRect(r);

	scene->setSceneRect(documentNode->boundingRect);
	
	//fitInView(sceneRect());
	//adjustSize();
}

void FormulaWnd::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Left:
		caret->MoveLeft();
		break;
	case Qt::Key_Right:
		caret->MoveRight();
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		if (commandManager.InsertLine())
			UpdateView();
		break;
	case Qt::Key_Delete:
		if (commandManager.Remove(true))
				UpdateView();
		break;
	case Qt::Key_Backspace:
		if (commandManager.Remove(false))
				UpdateView();
		break;
	case Qt::Key_Plus:
		if (commandManager.InsertNode(NULL, CommandAction(caret->currentState, &FormulaNode::DoCreatePlusFormulaNode)))
				UpdateView();
		break;
	case Qt::Key_Slash:
		if (commandManager.InsertNode(NULL, CommandAction(caret->currentState, &FormulaNode::DoCreateDivisionFormulaNode)))
			UpdateView();
		break;
	case Qt::Key_Equal:
		if (commandManager.InsertNode(NULL, CommandAction(caret->currentState, &FormulaNode::DoCreateEquationFormulaNode)))
			UpdateView();
		break;
	default:
		QString str = event->text();
		if (!str.isEmpty())
		{
			if (commandManager.InsertText(str, CommandAction(caret->currentState, &FormulaNode::DoInsertText)))
				UpdateView();
		}
		break;
	}
}

bool FormulaWnd::Open(QString fileName)
{
	DocumentFormulaNode* d = NULL;
	//DocumentFormulaNode* d = new DocumentFormulaNode(this);

	try
	{
		std::ifstream ifs((const char*)fileName.toUtf8().data(), ios::binary);
		//WndStream ifs((const char*)fileName.toUtf8().data());
		boost::archive::binary_iarchive ia(ifs);
		RegisterTypes<boost::archive::binary_iarchive>(ia);
		//firstly load into the temporary variable
		//ia >> c;
		ia >> d;
	}
	catch (...)
	{
		//if (c)
		//	delete c;
		
		QMessageBox msgBox(QMessageBox::Critical, "Natural Editor", "File open error!", QMessageBox::Ok);
		msgBox.exec();
		return false;
	}
	
	//in case of succesfull loading replace the document's child nodes
	delete documentNode;
	documentNode = d;
	//documentNode->childNodes->Clear();
	//documentNode->childNodes->CopyFrom(c);
	//documentNode->childNodes->CopyFrom(*d->childNodes);

	documentNode->Remake();
	caret->SetToNodeBegin(documentNode);
	caret->Render();
	
	commandManager.Reset();

	UpdateView();
	
	return true;
}

bool FormulaWnd::Save(QString fileName)
{
	try
	{
		std::ofstream ofs((const char*)fileName.toUtf8().data(), ios::binary);
		boost::archive::binary_oarchive oa(ofs);
		RegisterTypes<boost::archive::binary_oarchive>(oa);
		//save the document
		oa << documentNode;
	}
	catch (...)
	{
		QMessageBox msgBox(QMessageBox::Critical, "Natural Editor", "File save error!", QMessageBox::Ok);
		msgBox.exec();
		return false;
	}
	
	return true;
}

void FormulaWnd::Undo()
{
	commandManager.Undo();
	documentNode->Remake();
	caret->Render();
}

void FormulaWnd::Redo()
{
	commandManager.Redo();
	documentNode->Remake();
	caret->Render();
}

void FormulaWnd::UpdateView()
{
	documentNode->Remake();
	caret->Render();
	scene->setSceneRect(documentNode->boundingRect);
	
	FormulaNode* n = caret->currentState->GetNode();
	QRectF r = n->GetDocumentBounds();
	ensureVisible(r);
}
