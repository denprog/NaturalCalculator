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

int FormulaWnd::updateEventId;

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

	updateEventId = QEvent::registerEventType(1);
	
	parserThread = new ParserThread(this);
}

FormulaWnd::~FormulaWnd()
{
	delete caret;
	delete documentNode;
	delete scene;
	delete parserThread;
}

bool FormulaWnd::event(QEvent* e)
{
	if (e->type() == (QEvent::Type)updateEventId)
	{
		UpdateView();
		return true;
	}
	
	return QGraphicsView::event(e);
}

void FormulaWnd::resizeEvent(QResizeEvent* event)
{
	QGraphicsView::resizeEvent(event);
	scene->setSceneRect(documentNode->boundingRect);
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
		InsertNode(&FormulaNode::DoCreatePlusFormulaNode);
		break;
	case Qt::Key_Minus:
		InsertNode(&FormulaNode::DoCreateMinusFormulaNode);
		break;
	case Qt::Key_Asterisk:
		InsertNode(&FormulaNode::DoCreateMultiplyFormulaNode);
		break;
	case Qt::Key_Slash:
		InsertNode(&FormulaNode::DoCreateDivisionFormulaNode);
		break;
	case Qt::Key_Backslash:
		InsertNode(&FormulaNode::DoCreateSquareRootFormulaNode);
		break;
	case Qt::Key_Equal:
		InsertNode(&FormulaNode::DoCreateEquationFormulaNode);
		break;
	default:
		QString str = event->text();
		if (str == "^")
		{
			InsertNode(&FormulaNode::DoCreatePowerFormulaNode);
			break;
		}
		if (!str.isEmpty())
		{
			for (int i = 0; i < str.length(); ++i)
			{
				if (!str[i].isLetter() && !str[i].isDigit())
					return;
			}
			
			if (commandManager.InsertText(str, CommandAction(caret->currentState, &FormulaNode::DoInsertText)))
				UpdateView();
		}
		break;
	}
}

bool FormulaWnd::Open(QString fileName)
{
	DocumentFormulaNode* d = NULL;

	try
	{
		std::ifstream ifs((const char*)fileName.toUtf8().data(), ios::binary);
		boost::archive::binary_iarchive ia(ifs);
		RegisterTypes<boost::archive::binary_iarchive>(ia);
		//firstly load into the temporary variable
		ia >> d;
	}
	catch (...)
	{
		QMessageBox msgBox(QMessageBox::Critical, "Natural Editor", "File open error!", QMessageBox::Ok);
		msgBox.exec();
		return false;
	}
	
	//in case of succesfull loading replace the document's child nodes
	delete documentNode;
	documentNode = d;

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

void FormulaWnd::New()
{
	documentNode->Clear();
	caret->SetToNodeBegin(documentNode);
	commandManager.Reset();
	UpdateView();
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

void FormulaWnd::InsertNode(boost::function<bool (FormulaNode*, NodeEvent&)> action)
{
	if (commandManager.InsertNode(NULL, CommandAction(caret->currentState, action)))
		UpdateView();
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
