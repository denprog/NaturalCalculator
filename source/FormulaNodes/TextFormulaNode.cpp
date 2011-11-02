#include <QFontMetrics>
#include "TextFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "PlusFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "PowerFormulaNode.h"
#include "SquareRootFormulaNode.h"
#include "../Main/FormulaWnd.h"
#include "../Util/QRectEx.h"

/**
 * Default constructor.
 */
TextFormulaNode::TextFormulaNode()
{
}

/**
 * Constructor.
 * @param [in,out] parent The parent node.
 */
TextFormulaNode::TextFormulaNode(FormulaNode* parent) : FormulaNode(parent, parent->GetWnd())
{
	item = new FormulaTextItem(settings, level, boundingRect, parent->GetItem());
	item->setData(0, qVariantFromValue((void*)this));
	
	QFont font = settings->GetTextFormulaNodeFont(level);
	((QGraphicsTextItem*)item)->setFont(font);

#ifdef _DEBUG
	name = "TextFormulaNode";
#endif
}

/**
 * Destructor.
 */
TextFormulaNode::~TextFormulaNode()
{
}

/**
 * Remakes this node.
 */
void TextFormulaNode::Remake()
{
	((FormulaTextItem*)item)->level = level;
	UpdateBoundingRect();
	baseline = ((FormulaTextItem*)item)->font().pointSize();
}

/**
 * Updates the bounding rectangle.
 */
void TextFormulaNode::UpdateBoundingRect()
{
	QFont& font = settings->GetTextFormulaNodeFont(level);
	((FormulaTextItem*)item)->setFont(font);
	QFontMetrics m(font);
	QRectF b = m.boundingRect(((QGraphicsTextItem*)item)->toPlainText());
	
	boundingRect.setCoords(0, 0, b.width(), b.height());
	((FormulaTextItem*)item)->boundingRect = boundingRect;
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Makes a deep copy of this object.
 * @return A copy of this object.
 */
FormulaNode* TextFormulaNode::Clone()
{
	TextFormulaNode* res = new TextFormulaNode(parent);
	res->SetText(GetText());
	
	return res;
}

/**
 * Sets a text.
 * @param _text The text.
 */
void TextFormulaNode::SetText(QString _text)
{
	((QGraphicsTextItem*)item)->setPlainText(_text);
}

/**
 * Gets the text.
 * @return The text.
 */
QString TextFormulaNode::GetText()
{
	return ((QGraphicsTextItem*)item)->toPlainText();
}

/**
 * Gets the first caret position of the node.
 * @return The first caret position.
 */
SharedCaretState TextFormulaNode::GetFirstPosition()
{
	return SharedCaretState(new CaretState(this, 0));
}

/**
 * Gets the last caret position of the node.
 * @return The last position.
 */
SharedCaretState TextFormulaNode::GetLastPosition()
{
	return SharedCaretState(new CaretState(this, GetText().length()));
}

/**
 * Gets the next caret position of the node.
 * @param [in,out] relativeState Relative caret state.
 * @return The next caret position.
 */
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

/**
 * Gets the previous caret position of the node.
 * @param [in,out] relativeState Relative caret state.
 * @return The previous caret position.
 */
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

/**
 * Gets a position bounds relatively to the document.
 * @param pos The position.
 * @return The position bounds.
 */
QRectF TextFormulaNode::GetDocumentPosBounds(int pos)
{
	QRectF r = parent->GetDocumentPosBounds(parent->GetChildPos(this));

	//if (pos > 0)
	//{
		QFont font = settings->GetTextFormulaNodeFont(level);
		QFontMetrics m(font);
		QString text = ((QGraphicsTextItem*)item)->toPlainText();
		QRectF b = m.boundingRect(text.right(text.length() - pos));
		r.setLeft(r.left() + r.width() - b.width());
		r.setRight(r.left());
	//}
	
	return r;
}

/**
 * Gets a nearest position to the given point in the document.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The nearest position.
 */
int TextFormulaNode::GetNearestPos(qreal x, qreal y)
{
	int minDist = std::numeric_limits<int>::max();
	int res = 0;

	for (int i = 0; i <= GetText().length(); ++i)
	{
		QRectEx rect(GetDocumentPosBounds(i));
		int j = rect.DistToPoint(x, y);
		if (j < minDist)
		{
			minDist = j;
			res = i;
		}
	}
	
	return res;
}

/**
 * Renders the caret inside this node.
 * @param pos	The caret position.
 * @param anchor The anchor.
 */
void TextFormulaNode::RenderCaret(const int pos, const int anchor)
{
	QRectF r = GetDocumentPosBounds(pos);
	QGraphicsItemGroup* g = wnd->GetCaret()->caretShape;
	
	qDeleteAll(g->childItems());
	g->childItems().clear();
	
	QGraphicsLineItem* i = new QGraphicsLineItem(r.left(), r.top(), r.left(), r.bottom(), g);
	i->setPen(QPen("red"));
	g->addToGroup(i);
}

/**
 * Adds the current string to the expression.
 * @param [in,out] expr The expression.
 */
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

/**
 * Executes the insert text operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoInsertText(NodeEvent& nodeEvent)
{
	command = any_cast<Command*>(nodeEvent["command"]);
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

/**
 * Undo insert text.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoInsertText(NodeEvent& nodeEvent)
{
	command = any_cast<Command*>(nodeEvent["command"]);
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

/**
 * Executes the remove item operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoRemoveItem(NodeEvent& nodeEvent)
{
	command = any_cast<Command*>(nodeEvent["command"]);
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

/**
 * Undo remove item.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoRemoveItem(NodeEvent& nodeEvent)
{
	command = any_cast<Command*>(nodeEvent["command"]);
	bool right = any_cast<bool>(nodeEvent["right"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();
	QString str = any_cast<QString>(command->GetParam(this, "str"));
	QString text = GetText();
	
	text = text.left(pos) + str + text.right(text.length() - pos);
	SetText(text);
	
	return true;
}

/**
 * Executes the create plus formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreatePlusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a plus node and insert it into the parent after or before this node
	FormulaNode* p = new PlusFormulaNode(this, wnd);
	command = any_cast<Command*>(nodeEvent["command"]);
	if (c->GetPos() == 0)
	{
		parent->InsertChild(p, parent->GetChildPos(this));
		command->SetParam(this, "right", false);
		c->SetToNode(this, c->GetPos());
	}
	else
	{
		parent->InsertChild(p, parent->GetChildPos(this) + 1);
		command->SetParam(this, "right", true);
		c->SetToNode(parent, parent->GetChildPos(p) + 1);
	}

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreatePlusFormulaNode);

	return true;
}

/**
 * Undo create plus formula node.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoCreatePlusFormulaNode(NodeEvent& nodeEvent)
{
	bool right = any_cast<bool>(command->GetParam(this, "right"));
	parent->RemoveChild(right ? parent->GetChildPos(this) + 1 : parent->GetChildPos(this) - 1);
	
	return true;
}

/**
 * Executes the create minus formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateMinusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a minus node and insert it into the parent after or before this node
	FormulaNode* p = new MinusFormulaNode(this, wnd);
	command = any_cast<Command*>(nodeEvent["command"]);
	if (c->GetPos() == 0)
	{
		parent->InsertChild(p, parent->GetChildPos(this));
		command->SetParam(this, "right", false);
		c->SetToNode(this, c->GetPos());
	}
	else
	{
		parent->InsertChild(p, parent->GetChildPos(this) + 1);
		command->SetParam(this, "right", true);
		c->SetToNode(parent, parent->GetChildPos(p) + 1);
	}

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreatePlusFormulaNode);

	return true;
}

/**
 * Undo create minus formula node.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoCreateMinusFormulaNode(NodeEvent& nodeEvent)
{
	bool right = any_cast<bool>(nodeEvent["right"]);
	parent->RemoveChild(right ? parent->GetChildPos(this) + 1 : parent->GetChildPos(this) - 1);
	
	return true;
}

/**
 * Executes the create multiply formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateMultiplyFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a multiply node and insert it into the parent after or before this node
	FormulaNode* p = new MultiplyFormulaNode(this, wnd);
	command = any_cast<Command*>(nodeEvent["command"]);
	if (c->GetPos() == 0)
	{
		parent->InsertChild(p, parent->GetChildPos(this));
		command->SetParam(this, "right", false);
		c->SetToNode(this, c->GetPos());
	}
	else
	{
		parent->InsertChild(p, parent->GetChildPos(this) + 1);
		command->SetParam(this, "right", true);
		c->SetToNode(parent, parent->GetChildPos(p) + 1);
	}

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreatePlusFormulaNode);

	return true;
}

/**
 * Undo create multiply formula node.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoCreateMultiplyFormulaNode(NodeEvent& nodeEvent)
{
	bool right = any_cast<bool>(nodeEvent["right"]);
	parent->RemoveChild(right ? parent->GetChildPos(this) + 1 : parent->GetChildPos(this) - 1);
	
	return true;
}

/**
 * Executes the create division formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
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

/**
 * Undo create division formula node.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoCreateDivisionFormulaNode(NodeEvent& nodeEvent)
{
	FormulaNode* p = parent->GetParent()->GetParent();
	int pos = p->GetChildPos(parent->GetParent());
	p->MoveChild(this, pos);
	p->RemoveChild(pos + 1);
	
	return true;
}

/**
 * Executes the create power formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
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

/**
 * Undo create power formula node.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	FormulaNode* p = parent->GetParent()->GetParent();
	int pos = p->GetChildPos(parent->GetParent());
	p->MoveChild(this, pos);
	p->RemoveChild(pos + 1);
	
	return true;
}

/**
 * Executes the create square root formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* p = parent;
	int pos = parent->GetChildPos(this);
	//create a power node, insert current node into it and insert the result into the parent
	FormulaNode* d = new SquareRootFormulaNode(parent, wnd);
	FormulaNode* g = new GroupFormulaNode(d, wnd);
	g->MoveChild(this, 0);
	d->InsertChild(g, 1);
	p->InsertChild(d, pos);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateSquareRootFormulaNode);
	c->SetToNode(this, c->GetPos());
	
	return true;
}

/**
 * Undo create square root formula node.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::UndoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	FormulaNode* p = parent->GetParent()->GetParent();
	int pos = p->GetChildPos(parent->GetParent());
	p->MoveChild(this, pos);
	p->RemoveChild(pos + 1);
	
	return true;
}

//FormulaTextItem

/**
 * Constructor.
 * @param [in,out] _settings Application's settings.
 * @param _level The node level.
 * @param [in,out] _boundingRect The bounding rectangle.
 * @param [in,out] parent The parent item.
 */
FormulaTextItem::FormulaTextItem(Settings* _settings, FormulaNodeLevel _level, QRectF& _boundingRect, QGraphicsItem* parent) : 
	QGraphicsTextItem(parent), settings(_settings), level(_level), boundingRect(_boundingRect)
{
}

/**
 * Paints the current text.
 * @param [in,out] painter The painter.
 * @param option The options.
 * @param [in,out] widget The widget for the painting.
 */
void FormulaTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QFont& font = settings->GetTextFormulaNodeFont(level);
	painter->setFont(font);
	painter->drawText(boundingRect, Qt::AlignVCenter | Qt::AlignLeft, toPlainText());
}
