#include <QFontMetrics>
#include "TextFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "PlusFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "PowerFormulaNode.h"
#include "../Main/FormulaWnd.h"

TextFormulaNode::TextFormulaNode()
{
}

TextFormulaNode::TextFormulaNode(FormulaNode* parent) : FormulaNode(parent, parent->wnd)
{
	item = new FormulaTextItem(settings, level, boundingRect, parent->item);
	
	QFont font = settings->GetTextFormulaNodeFont(level);
	((QGraphicsTextItem*)item)->setFont(font);

#ifdef _DEBUG
	name = "TextFormulaNode";
#endif
}

TextFormulaNode::~TextFormulaNode()
{
}

void TextFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRectF b = m.boundingRect(((QGraphicsTextItem*)item)->toPlainText());
	
	boundingRect.setCoords(0, 0, b.width(), b.height());
	((FormulaTextItem*)item)->boundingRect = boundingRect;
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

void TextFormulaNode::Remake()
{
	((FormulaTextItem*)item)->level = level;
	UpdateBoundingRect();
	baseline = ((QGraphicsTextItem*)item)->font().pointSize();
}

FormulaNode* TextFormulaNode::Clone()
{
	TextFormulaNode* res = new TextFormulaNode(parent);
	res->SetText(GetText());
	
	return res;
}

void TextFormulaNode::SetText(QString _text)
{
	((QGraphicsTextItem*)item)->setPlainText(_text);
}

QString TextFormulaNode::GetText()
{
	return ((QGraphicsTextItem*)item)->toPlainText();
}

//void TextFormulaNode::SetFont(QFont font)
//{
//	((QGraphicsTextItem*)item)->setFont(font);
//}

SharedCaretState TextFormulaNode::GetFirstPosition()
{
	return SharedCaretState(new CaretState(this, 0));
}

SharedCaretState TextFormulaNode::GetLastPosition()
{
	return SharedCaretState(new CaretState(this, GetText().length()));
}

SharedCaretState TextFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	if (relativeState && relativeState->CheckInNode(this))
	{
		int pos = relativeState->GetPos();
		if (pos < GetText().length())
			return SharedCaretState(new CaretState(this, pos + 1));
		return parent->GetNextPosition(relativeState);
	}
	
	return GetFirstPosition();
}

SharedCaretState TextFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	if (relativeState && relativeState->CheckInNode(this))
	{
		int pos = relativeState->GetPos();
		if (pos > 0)
			return SharedCaretState(new CaretState(this, pos - 1));
		return parent->GetPreviousPosition(relativeState);
	}
	
	return GetLastPosition();
}

QRectF TextFormulaNode::GetDocumentPosBounds(int pos)
{
	QRectF r = parent->GetDocumentPosBounds(parent->GetChildPos(this));

	if (pos > 0)
	{
		QFont font = settings->GetTextFormulaNodeFont(level);
		QFontMetrics m(font);
		QString text = ((QGraphicsTextItem*)item)->toPlainText();
		QRectF b = m.boundingRect(text.right(text.length() - pos));
		r.setLeft(r.left() + r.width() - b.width());
		r.setRight(r.left());
	}
	
	return r;
}

void TextFormulaNode::RenderCaret(const int pos, const int anchor)
{
	QRectF r = GetDocumentPosBounds(pos);
	
	QGraphicsItemGroup* g = wnd->caret->caretShape;
	
	qDeleteAll(g->childItems());
	g->childItems().clear();
	
	QGraphicsLineItem* i = new QGraphicsLineItem(r.left(), r.top(), r.left(), r.bottom(), g);
	i->setPen(QPen("red"));
	g->addToGroup(i);
}

void TextFormulaNode::Parse(ParserString& expr)
{
	expr.Add(GetText().toUtf8().data(), this);
}

bool TextFormulaNode::DoInsertNode(NodeEvent& nodeEvent)
{
	return false;
}

bool TextFormulaNode::UndoInsertNode(NodeEvent& nodeEvent)
{
	return false;
}

bool TextFormulaNode::DoInsertText(NodeEvent& nodeEvent)
{
	Command* command = any_cast<Command*>(nodeEvent["command"]);
	QString str = any_cast<QString>(nodeEvent["text"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();
	QString text = GetText();
	
	command->SetParam(this, "pos", pos);
	
	//update the item
	text = text.left(pos) + str + text.right(text.length() - pos);
	SetText(text);
	
	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoInsertText);
	c->SetToNode(this, pos + str.length());
	
	return true;
}

bool TextFormulaNode::UndoInsertText(NodeEvent& nodeEvent)
{
	Command* command = any_cast<Command*>(nodeEvent["command"]);
	int pos = any_cast<int>(command->GetParam(this, "pos"));
	QString str = any_cast<QString>(nodeEvent["text"]);
	QString text = GetText();
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);

	//update the item	
	text = text.left(pos) + text.right(text.length() - pos - str.length());
	SetText(text);
	
	c->SetToNode(this, pos);
	
	return true;
}

bool TextFormulaNode::DoRemoveItem(NodeEvent& nodeEvent)
{
	Command* command = any_cast<Command*>(nodeEvent["command"]);
	bool right = any_cast<bool>(nodeEvent["right"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();
	QString text = GetText();
	
	if (right)
	{
		if (pos < text.length())
		{
			command->SetParam(this, "str", QString(text.mid(pos, 1)));
			//update the item
			text = text.left(pos) + text.right(text.length() - pos - 1);
			SetText(text);
			c->SetToNode(this, pos);
		}
		else
			return false;
	}
	else
	{
		if (pos > 0)
		{
			command->SetParam(this, "str", QString(text.mid(pos - 1, 1)));
			//update the item
			text = text.left(pos - 1) + text.right(text.length() - pos);
			SetText(text);
			c->SetToNode(this, pos - 1);
		}
		else
			return false;
	}

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoRemoveItem);
	
	return true;
}

bool TextFormulaNode::UndoRemoveItem(NodeEvent& nodeEvent)
{
	Command* command = any_cast<Command*>(nodeEvent["command"]);
	bool right = any_cast<bool>(nodeEvent["right"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();
	QString str = any_cast<QString>(command->GetParam(this, "str"));
	QString text = GetText();
	
	text = text.left(pos) + str + text.right(text.length() - pos);
	SetText(text);
	
	return true;
}

bool TextFormulaNode::DoCreatePlusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a plus node and insert it into the parent after this node
	FormulaNode* p = new PlusFormulaNode(this, wnd);
	parent->InsertChild(p, parent->GetChildPos(this) + 1);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreatePlusFormulaNode);
	c->SetToNode(parent, parent->GetChildPos(p) + 1);

	return true;
}

bool TextFormulaNode::UndoCreatePlusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	parent->RemoveChild(parent->GetChildPos(this) + 1);
	
	return true;
}

bool TextFormulaNode::DoCreateMinusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a plus node and insert it into the parent after this node
	FormulaNode* p = new MinusFormulaNode(this, wnd);
	parent->InsertChild(p, parent->GetChildPos(this) + 1);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateMinusFormulaNode);
	c->SetToNode(parent, parent->GetChildPos(p) + 1);

	return true;
}

bool TextFormulaNode::UndoCreateMinusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	parent->RemoveChild(parent->GetChildPos(this) + 1);
	
	return true;
}

bool TextFormulaNode::DoCreateMultiplyFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a plus node and insert it into the parent after this node
	FormulaNode* p = new MultiplyFormulaNode(this, wnd);
	parent->InsertChild(p, parent->GetChildPos(this) + 1);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateMultiplyFormulaNode);
	c->SetToNode(parent, parent->GetChildPos(p) + 1);

	return true;
}

bool TextFormulaNode::UndoCreateMultiplyFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	parent->RemoveChild(parent->GetChildPos(this) + 1);
	
	return true;
}

bool TextFormulaNode::DoCreateDivisionFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* p = parent;
	int pos = parent->GetChildPos(this);
	//create a division node, insert current node into it and insert it into the parent
	FormulaNode* d = new DivisionFormulaNode(parent, wnd);
	FormulaNode* g = new GroupFormulaNode(d, wnd);
	g->MoveChild(this, 0);
	d->AddChild(g);
	g = new GroupFormulaNode(d, wnd);
	FormulaNode* n = new EmptyFormulaNode(g);
	g->AddChild(n);
	d->AddChild(g);
	p->InsertChild(d, pos);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateDivisionFormulaNode);
	c->SetToNode(g, 0);
	
	return true;
}

bool TextFormulaNode::UndoCreateDivisionFormulaNode(NodeEvent& nodeEvent)
{
	FormulaNode* p = parent->parent->parent;
	int pos = p->GetChildPos(parent->parent);
	p->MoveChild(this, pos);
	//delete p->(*this)[pos + 1];
	p->RemoveChild(pos + 1);
	
	return true;
}

bool TextFormulaNode::DoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* p = parent;
	int pos = parent->GetChildPos(this);
	//create a power node, insert current node into it and insert the result into the parent
	FormulaNode* d = new PowerFormulaNode(parent, wnd);
	FormulaNode* g = new GroupFormulaNode(d, wnd);
	g->MoveChild(this, 0);
	d->InsertChild(g, 0);
	g = new GroupFormulaNode(d, wnd);
	FormulaNode* n = new EmptyFormulaNode(g);
	g->AddChild(n);
	d->AddChild(g);
	p->InsertChild(d, pos);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreatePowerFormulaNode);
	c->SetToNode(g, 0);
	
	return true;
}

bool TextFormulaNode::UndoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	FormulaNode* p = parent->parent->parent;
	int pos = p->GetChildPos(parent->parent);
	p->MoveChild(this, pos);
	p->RemoveChild(pos + 1);
	
	return true;
}

bool TextFormulaNode::DoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

bool TextFormulaNode::UndoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

//FormulaTextItem

FormulaTextItem::FormulaTextItem(Settings* _settings, FormulaNodeLevel _level, QRectF& _boundingRect, QGraphicsItem* parent) : 
	QGraphicsTextItem(parent), settings(_settings), level(_level), boundingRect(_boundingRect)
{
}

FormulaTextItem::~FormulaTextItem()
{
}

void FormulaTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	painter->setFont(font);
	painter->drawText(boundingRect, Qt::AlignVCenter | Qt::AlignLeft, toPlainText());
}
