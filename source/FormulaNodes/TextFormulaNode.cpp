#include <QFontMetrics>
#include <boost/regex.hpp>
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
	type = TEXT_NODE;
}

TextFormulaNode::TextFormulaNode(FormulaNode* parent) : FormulaNode(parent, parent->wnd)
{
	type = TEXT_NODE;
	item = new FormulaTextItem(settings, level, boundingRect, parent->item);
	item->setData(0, qVariantFromValue((void*)this));
	
	QFont font = settings->GetTextFormulaNodeFont(level);
	((QGraphicsTextItem*)item)->setFont(font);
}

/**
 * Constructor.
 * @param [in,out] parent The parent node.
 * @param [in,out] wnd The parent window.
 */
TextFormulaNode::TextFormulaNode(FormulaNode* parent, FormulaWnd* wnd) : FormulaNode(parent, wnd)
{
	type = TEXT_NODE;
	item = new FormulaTextItem(settings, level, boundingRect, parent ? parent->item : NULL);
	item->setData(0, qVariantFromValue((void*)this));
	
	QFont font = settings->GetTextFormulaNodeFont(level);
	((QGraphicsTextItem*)item)->setFont(font);
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
	
	boundingRect.setCoords(0, 0, m.width(((QGraphicsTextItem*)item)->toPlainText()), b.height());
	((FormulaTextItem*)item)->boundingRect = boundingRect;
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* TextFormulaNode::Clone(FormulaNode* p)
{
	TextFormulaNode* res = new TextFormulaNode(p, wnd);
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
SharedCaretState TextFormulaNode::GetNextPosition(SharedCaretState relativeState)
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
SharedCaretState TextFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
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
 * Returns the first caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState TextFormulaNode::GetLineBegin(SharedCaretState& relativeState)
{
	return parent->GetLineBegin(relativeState);
}

/**
 * Returns the end caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState TextFormulaNode::GetLineEnd(SharedCaretState& relativeState)
{
	return parent->GetLineEnd(relativeState);
}

/**
 * Gets a position bounds relatively to the document.
 * @param pos The position.
 * @return The position bounds.
 */
QRectF TextFormulaNode::GetDocumentPosBounds(int pos)
{
	QRectF r = parent->GetDocumentPosBounds(parent->GetChildPos(this));

	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QString text = ((QGraphicsTextItem*)item)->toPlainText();
	r.setLeft(r.left() + m.width(text.left(pos)));
	r.setRight(r.left());
	
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

#ifdef TEST
std::string TextFormulaNode::ParseStructure()
{
	return GetText().toUtf8().data();
}
#endif

bool TextFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	std::string res;
	std::string::iterator i = begin;
	while (i != end)
	{
		if (boost::regex_match(i, i + 1, boost::regex("[0-9a-zA-Z.]")))
		{
			res += *i;
			++i;
		}
		else
			break;
	}
	
	if (!res.empty())
	{
		begin = i;
		TextFormulaNode* t = new TextFormulaNode(parent);
		t->SetText(res.c_str());
		parent->AddChild(t);
		return true;
	}
	
	return false;
}

std::string TextFormulaNode::ToString()
{
	return GetText().toUtf8().data();
}

bool TextFormulaNode::DoInsertNode(Command* command)
{
	return false;
}

bool TextFormulaNode::UndoInsertNode(Command* command)
{
	return false;
}

/**
 * Executes the insert text operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoInsertText(Command* command)
{
	command->SaveNodeState(parent);
	
	QString str = any_cast<QString>(command->nodeEvent["text"]);
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	
	int pos = c->GetPos();
	QString text = GetText();
	//update the item
	text = text.left(pos) + str + text.right(text.length() - pos);
	SetText(text);
	c->SetToNode(this, pos + str.length());
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the remove item operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoRemoveItem(Command* command)
{
	command->SaveNodeState(parent);
	
	bool right = any_cast<bool>(command->nodeEvent["right"]);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	QString text = GetText();
	
	if (right)
	{
		if (pos < text.length())
		{
			//update the item
			text = text.left(pos) + text.right(text.length() - pos - 1);
			if (text == "")
			{
				int i = parent->GetFirstLevelChildPos(this);
				FormulaNode* p = parent;
				if (p->ChildrenCount() == 1)
				{
					p->InsertChild(new EmptyFormulaNode(this), i++);
					p->RemoveChild(i);
					c->SetToNode(p, 0);
				}
				else
				{
					p->RemoveChild(i);
					c->SetToNode(p, i);
				}
			}
			else
			{
				SetText(text);
				c->SetToNode(this, pos);
			}
		}
		else
			return false;
	}
	else
	{
		if (pos > 0)
		{
			//update the item
			text = text.left(pos - 1) + text.right(text.length() - pos);
			if (text == "")
			{
				int i = parent->GetFirstLevelChildPos(this);
				FormulaNode* p = parent;
				if (p->ChildrenCount() == 1)
				{
					p->InsertChild(new EmptyFormulaNode(this), i++);
					p->RemoveChild(i);
					c->SetToNode(p, 0);
				}
				else
				{
					p->RemoveChild(i);
					c->SetToNode(p, i);
				}
			}
			else
			{
				SetText(text);
				c->SetToNode(this, pos - 1);
			}
		}
		else
			return false;
	}

	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create plus formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreatePlusFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	
	command->SaveNodeState(parent);

	//create a plus node and insert it into the parent after or before this node
	FormulaNode* p = new PlusFormulaNode(this, wnd);
	if (c->GetPos() == 0)
	{
		parent->InsertChild(p, parent->GetChildPos(this));
		c->SetToNode(this, c->GetPos());
	}
	else
	{
		parent->InsertChild(p, parent->GetChildPos(this) + 1);
		c->SetToNode(parent, parent->GetChildPos(p) + 1);
	}
	
	command->afterCaretState = c;

	return true;
}

/**
 * Executes the create minus formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateMinusFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	
	command->SaveNodeState(parent);

	//create a minus node and insert it into the parent after or before this node
	FormulaNode* p = new MinusFormulaNode(this, wnd);
	if (c->GetPos() == 0)
	{
		parent->InsertChild(p, parent->GetChildPos(this));
		c->SetToNode(this, c->GetPos());
	}
	else
	{
		parent->InsertChild(p, parent->GetChildPos(this) + 1);
		c->SetToNode(parent, parent->GetChildPos(p) + 1);
	}
	
	command->afterCaretState = c;

	return true;
}

/**
 * Executes the create multiply formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateMultiplyFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	
	command->SaveNodeState(parent);

	//create a multiply node and insert it into the parent after or before this node
	FormulaNode* p = new MultiplyFormulaNode(this, wnd);
	if (c->GetPos() == 0)
	{
		parent->InsertChild(p, parent->GetChildPos(this));
		c->SetToNode(this, c->GetPos());
	}
	else
	{
		parent->InsertChild(p, parent->GetChildPos(this) + 1);
		c->SetToNode(parent, parent->GetChildPos(p) + 1);
	}
	
	command->afterCaretState = c;

	return true;
}

/**
 * Executes the create division formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateDivisionFormulaNode(Command* command)
{
	command->SaveNodeState(parent);

	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* p = parent;
	int pos = parent->GetChildPos(this);
	//create a division node, insert current node into it and insert it into the parent
	DivisionFormulaNode* d = new DivisionFormulaNode(parent, wnd);
	d->dividend->MoveChild(this, 0);
	d->divisor->AddChild(new EmptyFormulaNode(d->divisor));
	p->InsertChild(d, pos);
	c->SetToNode(d->divisor, 0);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create power formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreatePowerFormulaNode(Command* command)
{
	command->SaveNodeState(parent);

	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* p = parent;
	int pos = parent->GetChildPos(this);
	//create a power node, insert current node into it and insert the result into the parent
	PowerFormulaNode* n = new PowerFormulaNode(parent, wnd);
	n->base->MoveChild(this, 0);
	n->exponent->AddChild(new EmptyFormulaNode(n->exponent));
	p->InsertChild(n, pos);
	c->SetToNode(n->exponent, 0);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create square root formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateSquareRootFormulaNode(Command* command)
{
	command->SaveNodeState(parent);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* p = parent;
	int pos = parent->GetChildPos(this);
	//create a power node, insert current node into it and insert the result into the parent
	FormulaNode* d = new SquareRootFormulaNode(parent, wnd);
	FormulaNode* expr = d->GetExpression(0);
	expr->MoveChild(this, 0);
	p->InsertChild(d, pos);

	c->SetToNode(this, c->GetPos());
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create left brace formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateLeftBraceFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* node = c->GetNode()->parent;
	int pos = node->GetFirstLevelChildPos(this);
	if (c->GetPos() == GetText().length())
		++pos;
	
	if (node->parent->type == BRACES_NODE && node->parent->IsShapeVisible(1))
	{
		command->SaveNodeState(parent->parent);
		
		FormulaNode* p = node->parent->parent;
		//this is a braces node with a right brace, set a left brace
		node->parent->ShowShape(0, true);
		
		//move the remain child nodes after the caret to the parent node
		for (int i = pos - 1; i >= 0; --i)
			p->MoveChild((*node)[i], 0);

		c->SetToNode(p, pos);
		command->afterCaretState = c;
	
		return true;
	}

	command->SaveNodeState(parent);

	//create a braces node, insert the nodes which are located to the right in the collection, into it and insert the result into the parent
	FormulaNode* d = new BracesFormulaNode(node->parent, wnd, true, false);
	FormulaNode* expr = d->GetExpression(0);
	for (int i = pos; i < node->ChildrenCount();)
		expr->MoveChild((*node)[pos], expr->ChildrenCount());
	if (expr->ChildrenCount() == 0)
		expr->AddChild(new EmptyFormulaNode(expr));
	node->InsertChild(d, pos);

	c->SetToNode(expr, 0);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create right brace formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool TextFormulaNode::DoCreateRightBraceFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* node = c->GetNode()->parent;
	int pos = node->GetFirstLevelChildPos(this);
	if (c->GetPos() == GetText().length())
		++pos;
	
	if (node->parent->type == BRACES_NODE && node->parent->IsShapeVisible(0))
	{
		command->SaveNodeState(parent->parent);
		
		FormulaNode* b = node->parent;
		FormulaNode* p = b->parent;
		int j = p->GetFirstLevelChildPos(b);
		//this is a braces node with a left brace, set a right brace
		b->ShowShape(1, true);
		
		//move the remain child nodes after the caret to the parent node
		for (int i = pos; i < node->ChildrenCount();)
			p->MoveChild((*node)[i], p->ChildrenCount());

		c->SetToNode(p, j + 1);
		command->afterCaretState = c;
	
		return true;
	}

	if (parent->parent != NULL)
		command->SaveNodeState(parent->parent);
	else
		command->SaveNodeState(parent);
	
	//create a braces node, insert the nodes located to the left in the collection, into it and insert the result into the parent
	FormulaNode* d = new BracesFormulaNode(node, wnd, false, true);
	FormulaNode* expr = d->GetExpression(0);
	for (int i = pos - 1; i >= 0; --i)
		expr->MoveChild((*node)[i], 0);
	if (expr->ChildrenCount() == 0)
		expr->AddChild(new EmptyFormulaNode(expr));
	node->InsertChild(d, 0);

	if (c->GetPos() == 0)
		c->SetToNode(this, 0);
	else
		c->SetToNode(node->type == GROUP_NODE ? node->parent : node, 1);
	command->afterCaretState = c;
	
	return true;
}

void TextFormulaNode::OnCopy()
{
}

void TextFormulaNode::OnPaste()
{
}

void TextFormulaNode::OnCut()
{
}

//TextShapeFormulaNode

TextShapeFormulaNode::TextShapeFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, QString _text) : ShapeFormulaNode(_parent, wnd), text(_text)
{
	item = new FormulaTextItem(settings, level, boundingRect, _parent->item);
	item->setData(0, qVariantFromValue((void*)this));
	((QGraphicsTextItem*)item)->setPlainText(_text);
	
	QFont font = settings->GetTextFormulaNodeFont(level);
	((QGraphicsTextItem*)item)->setFont(font);
}
	
void TextShapeFormulaNode::Remake()
{
	((FormulaTextItem*)item)->level = level;
	UpdateBoundingRect();
	baseline = ((FormulaTextItem*)item)->font().pointSize();
}

void TextShapeFormulaNode::UpdateBoundingRect()
{
	QFont& font = settings->GetTextFormulaNodeFont(level);
	((FormulaTextItem*)item)->setFont(font);
	QFontMetrics m(font);
	QRectF b = m.boundingRect(((QGraphicsTextItem*)item)->toPlainText());
	
	boundingRect.setCoords(0, 0, m.width(((QGraphicsTextItem*)item)->toPlainText()), b.height());
	((FormulaTextItem*)item)->boundingRect = boundingRect;
	boundingRect.moveTo(item->pos().x(), item->pos().y());
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
