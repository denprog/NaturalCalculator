#include "FormulaNode.h"
#include "PlusFormulaNode.h"
#include "TextFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "PowerFormulaNode.h"
#include "SquareRootFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "BracesFormulaNode.h"
#include "CommaFormulaNode.h"
#include "../Main/FormulaWnd.h"
#include "../Util/QRectEx.h"
#include <QMenu>

/**
 * Default constructor.
 */
FormulaNode::FormulaNode()
{
	parent = NULL;
	wnd = NULL;
	item = NULL;
	baseline = 0;
	command = NULL;
	settings = NULL;
	childNodes = NULL;
	level = NORMAL_LEVEL;
}

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] _wnd	The formula window.
 */
FormulaNode::FormulaNode(FormulaNode* _parent, FormulaWnd* _wnd)
{
	parent = _parent;
	wnd = _wnd;
	item = NULL;
	baseline = 0;
	command = NULL;
	settings = _wnd->settings;
	childNodes = new FormulaNodesCollection();
	level = NORMAL_LEVEL;
}

/**
 * Destructor.
 */
FormulaNode::~FormulaNode()
{
	childNodes->Clear();

	if (item)
	{	
		qDeleteAll(item->childItems());
		item->childItems().clear();
		delete item;
	}
	
	delete childNodes;
}

/**
 * Adds a child node.
 * @param [in] node The node.
 */
void FormulaNode::AddChild(FormulaNode* node)
{
	childNodes->Add(node);
	node->item->setParentItem(item);
	node->parent = this;
}

/**
 * Inserts a child node.
 * @param [in] node The node.
 * @param pos The position.
 */
void FormulaNode::InsertChild(FormulaNode* node, int pos)
{
	childNodes->Insert(node, pos);
	node->item->setParentItem(item);
	node->parent = this;
}

/**
 * Move a node to the current collection.
 * @param [in,out] node The node.
 * @param pos The position.
 */
void FormulaNode::MoveChild(FormulaNode* node, int pos)
{
	int p = node->parent->GetChildPos(node);
	node->parent->childNodes->Remove(p);
	InsertChild(node, pos);
}

/**
 * Removes a child node.
 * @param pos The position of the node.
 */
void FormulaNode::RemoveChild(int pos)
{
	(*this)[pos]->item->setParentItem(NULL);
	delete (*this)[pos];
	childNodes->Remove(pos);
}

void FormulaNode::RemoveChild(FormulaNode* node)
{
	int pos = GetChildPos(node);
	assert(pos >= 0);
	
	return RemoveChild(pos);
}

void FormulaNode::RemoveChildNodes()
{
	for (int i = 0; i < childNodes->Count(); ++i)
		(*this)[i]->RemoveChildNodes();
	childNodes->Clear();
}

void FormulaNode::ReplaceChild(FormulaNode* node, int pos)
{
	RemoveChild(pos);
	InsertChild(node, pos);
	wnd->GetDocumentNode()->Remake();
}

void FormulaNode::Normalize()
{
	for (int i = 0; i < childNodes->Count(); ++i)
		(*this)[i]->Normalize();
}

/**
 * Returns the children count.
 * @return The children count.
 */
int FormulaNode::ChildrenCount()
{
	return childNodes->Count();
}

/**
 * Queries if one can insert a node or a text.
 * @param pos The position.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::CanInsert(int pos)
{
	return true;
}

/**
 * Queries if one can remove a node or a text.
 * @param pos The position.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::CanRemove(int pos)
{
	return true;
}

/**
 * Returns an expression at the pos.
 * @param pos The position.
 * @return null if it fails, else the expression.
 */
FormulaNode* FormulaNode::GetExpression(int pos) const
{
	return NULL;
}

/**
 * Shows the shape at the pos.
 * @param pos The position.
 * @param show true to show, false to hide.
 */
void FormulaNode::ShowShape(int pos, bool show)
{
}

/**
 * Query if a shape is visible at the pos.
 * @param pos The position.
 * @return true if shape visible, false if not.
 */
bool FormulaNode::IsShapeVisible(int pos) const
{
	return false;
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return The copy of this object.
 */
FormulaNode* FormulaNode::Clone(FormulaNode* p)
{
	FormulaNode* res = new FormulaNode(parent, wnd);
	res->childNodes->CopyFrom(*childNodes, res);
	
	return res;
}

/**
 * [] casting operator.
 * @param pos The position.
 * @return The child node at the position.
 */
FormulaNode* FormulaNode::operator[](int pos) const
{
	return (*childNodes)[pos];
}

/**
 * Remakes this node.
 */
void FormulaNode::Remake()
{
	childNodes->Remake();
}

/**
 * Updates this node.
 */
void FormulaNode::Update()
{
}

/**
 * Updates the bounding rectangle.
 */
void FormulaNode::UpdateBoundingRect()
{
	childNodes->UpdateBoundingRect();
	
	int w = 0, h = 0;
	
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		if (n->boundingRect.right() > w)
			w = n->boundingRect.right();
		if (n->boundingRect.bottom() > h)
			h = n->boundingRect.bottom();
	}

	if (w == 0 && h == 0 && item)
		boundingRect = item->boundingRect();
	else
		boundingRect.setRect(0, 0, w, h);
	
	if (item)
		boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Gets the lesser node level of this node.
 * @return The level.
 */
FormulaNodeLevel FormulaNode::GetLesserLevel()
{
	if (level == STILL_LESS_LEVEL)
		return STILL_LESS_LEVEL;
	return (FormulaNodeLevel)(level + 1);
}

/**
 * Gets the greater node level of this node.
 * @return The level.
 */
FormulaNodeLevel FormulaNode::GetGreaterLevel()
{
	if (level == NORMAL_LEVEL)
		return NORMAL_LEVEL;
	return (FormulaNodeLevel)(level - 1);
}

/**
 * Sets a node level.
 * @param _level The level.
 */
void FormulaNode::SetLevel(FormulaNodeLevel _level)
{
	for (int i = 0; i < childNodes->Count(); ++i)
		(*childNodes)[i]->SetLevel(_level);
	level = _level;
}

/**
 * Moves node to the coordinates.
 * @param x The x coordinate.
 * @param y The y coordinate.
 */
void FormulaNode::Move(int x, int y)
{
	item->setPos(x, y);
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Gets a hierarchy position of this node.
 * @param [out] positions The positions.
 */
void FormulaNode::GetHierarchyPos(HierarchyPos& positions) const
{
	const FormulaNode* n = this;
	while (n->parent)
	{
		positions.push_back(n->parent->GetChildPos(n));
		n = n->parent;
	}
}

/**
 * Parses the child nodes.
 * @param [in,out] expr The expression.
 */
void FormulaNode::Parse(ParserString& expr)
{
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->Parse(expr);
	}
}

#ifdef TEST
void FormulaNode::ParseStructure(QString& res)
{
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->ParseStructure(res);
	}
}
#endif

bool FormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (GroupFormulaNode::FromString(begin, end, parent))
		return true;
	if (PowerFormulaNode::FromString(begin, end, parent))
		return true;
	if (SquareRootFormulaNode::FromString(begin, end, parent))
		return true;
	if (CommaFormulaNode::FromString(begin, end, parent))
		return true;
	if (EmptyFormulaNode::FromString(begin, end, parent))
		return true;
	if (TextFormulaNode::FromString(begin, end, parent))
		return true;
	if (PlusFormulaNode::FromString(begin, end, parent))
		return true;
	if (MinusFormulaNode::FromString(begin, end, parent))
		return true;
	if (MultiplyFormulaNode::FromString(begin, end, parent))
		return true;
	if (DivisionFormulaNode::FromString(begin, end, parent))
		return true;
	if (BracesFormulaNode::FromString(begin, end, parent))
		return true;
	return false;
}

std::string FormulaNode::ToString()
{
	std::string res;
	for (int i = 0; i < ChildrenCount(); ++i)
		res += (*this)[i]->ToString();
	return res;
}

/**
 * Gets a child position of a node.
 * @param [in,out] node The node.
 * @return The child position.
 */
int FormulaNode::GetChildPos(const FormulaNode* node) const
{
	return childNodes->GetPos(node);
}

/**
 * Query if a node is a child.
 * @param node The node.
 * @return true if child, false if not.
 */
bool FormulaNode::IsChild(const FormulaNode* node)
{
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		if (n == node)
			return true;
		if (n->IsChild(node))
			return true;
	}
	
	return false;
}

/**
 * Gets the first level child position.
 * @param [in] node The node.
 * @return The first level child position.
 */
int FormulaNode::GetFirstLevelChildPos(FormulaNode* node)
{
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		if (n == node || n->GetFirstLevelChildPos(node) != -1)
			return i;
	}
	
	return -1;
}

/**
 * Gets the document bounds.
 * @return The document bounds.
 */
QRectF FormulaNode::GetDocumentBounds()
{
	return parent->GetDocumentPosBounds(parent->GetChildPos(this));
}

/**
 * Gets a position bounds in the document.
 * @param pos The position.
 * @return The document position bounds.
 */
QRectF FormulaNode::GetDocumentPosBounds(int pos)
{
	FormulaNode* n = (*this)[pos == childNodes->Count() ? pos - 1 : pos];
	FormulaNode* p = n;
	qreal cx = (pos == childNodes->Count() ? n->boundingRect.width() : 0);
	qreal cy = 0;
	
	while (p)
	{
		cx += p->boundingRect.left();
		cy += p->boundingRect.top();
		p = p->parent;
	}
	
	return QRectF(cx, cy, pos == childNodes->Count() ? 0 : n->boundingRect.width(), n->boundingRect.height());
}

/**
 * Gets a nearest position to the given point in the document.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The nearest position.
 */
int FormulaNode::GetNearestPos(qreal x, qreal y)
{
	int minDist = std::numeric_limits<int>::max();
	int res = -1;

	for (int i = 0; i < childNodes->Count(); ++i)
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
 * Query if this node is empty symbol.
 * @return true if empty symbol, false if not.
 */
bool FormulaNode::IsEmptySymbol()
{
	if (childNodes->Count() == 1 && (*childNodes)[0]->type == GROUP_NODE)
		return (*childNodes)[0]->IsEmptySymbol();
	return childNodes->Count() == 1 && (*this)[0]->type == EMPTY_NODE;
}

/**
 * Gets the first caret position of the node.
 * @return The first caret position.
 */
SharedCaretState FormulaNode::GetFirstPosition()
{
	if (childNodes->Count() > 0)
		return childNodes->GetFirst()->GetFirstPosition();
	return SharedCaretState();
}

/**
 * Gets the last caret position of the node.
 * @return The last caret position.
 */
SharedCaretState FormulaNode::GetLastPosition()
{
	if (childNodes->Count() > 0)
		return childNodes->GetLast()->GetLastPosition();
	return SharedCaretState();
}

/**
 * Gets the next caret position of the node.
 * @param [in,out] relativeState Relative caret state.
 * @return Empty caret state for FormulaNode object.
 */
SharedCaretState FormulaNode::GetNextPosition(SharedCaretState relativeState)
{
	return SharedCaretState();
}

/**
 * Gets the previous position of this node.
 * @param [in,out] relativeState Relative caret state.
 * @return Empty caret state for FormulaNode object.
 */
SharedCaretState FormulaNode::GetPreviousPosition(SharedCaretState relativeState)
{
	return SharedCaretState();
}

/**
 * Returns the first caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState FormulaNode::GetLineBegin(SharedCaretState& relativeState)
{
	return GetFirstPosition();
}

/**
 * Returns the end caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState FormulaNode::GetLineEnd(SharedCaretState& relativeState)
{
	return GetLastPosition();
}

/**
 * Queries if we can set caret on the node.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::CanSetCaret()
{
	return true;
}

/**
 * Renders the caret inside this node.
 * @param pos The position.
 * @param anchor The anchor.
 */
void FormulaNode::RenderCaret(const int pos, const int anchor)
{
	QRectF r = GetDocumentPosBounds(pos);
	
	QGraphicsItemGroup* g = wnd->GetCaret()->caretShape;
	
	qDeleteAll(g->childItems());
	g->childItems().clear();
	
	QGraphicsLineItem* i = new QGraphicsLineItem(r.left(), r.top(), r.left(), r.bottom(), g);
	i->setPen(QPen("red"));
	g->addToGroup(i);
	i = new QGraphicsLineItem(r.left(), r.bottom(), r.right(), r.bottom(), g);
	i->setPen(QPen("red"));
	g->addToGroup(i);
	g->setZValue(1);
}

bool FormulaNode::DoInsertNode(Command* command)
{
	return false;
}

/**
 * Executes the insert text operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoInsertText(Command* command)
{
	command->SaveNodeState(parent);
	
	QString str = any_cast<QString>(command->nodeEvent["text"]);
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* n = c->GetNode();
	int pos = c->GetPos();
	
	if (pos < n->childNodes->Count() && (*n)[pos]->type == EMPTY_NODE)
		RemoveChild(pos);
	
	//insert a text node
	TextFormulaNode* t = new TextFormulaNode(this);
	t->SetText(str);
	InsertChild(t, pos);
	
	c->SetToNode(t, 1);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the insert line operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoInsertLine(Command* command)
{
	if (parent)
		return parent->DoInsertLine(command);
	return false;
}

/**
 * Undo insert line.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoInsertLine(Command* command)
{
	if (parent)
		return parent->UndoInsertLine(command);
	return false;
}

/**
 * Executes the remove item operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoRemoveItem(Command* command)
{
	bool right = any_cast<bool>(command->nodeEvent["right"]);
	
	command->SaveNodeState(parent);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();

	if (right)
	{
		if (pos < childNodes->Count())
		{
			if (ChildrenCount() == 1 && (*this)[0]->IsEmptySymbol())
				return false;
			
			//remove the node
			RemoveChild(pos);
			if (childNodes->Count() == 0)
				AddChild(new EmptyFormulaNode(this));
			c->SetToNode(this, pos);
		}
		else
			return false;
	}
	else
	{
		if (pos > 0)
		{
			if (ChildrenCount() == 1 && (*this)[0]->IsEmptySymbol())
				return false;
			
			//remove the node
			RemoveChild(pos - 1);
			c->SetToNode(this, pos - 1);
		}
		else
			return false;
	}
	
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create plus formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreatePlusFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());

	command->SaveNodeState(parent);
	
	//create a plus node and insert it into the child nodes
	FormulaNode* p = new PlusFormulaNode(this, wnd);
	InsertChild(p, c->GetPos());
	c->SetToNode(this, c->GetPos() + 1);
	command->afterCaretState = c;

	return true;
}

/**
 * Executes the create minus formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateMinusFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());

	command->SaveNodeState(parent);
	
	//create a minus node and insert it into the child nodes
	FormulaNode* p = new MinusFormulaNode(this, wnd);
	InsertChild(p, c->GetPos());
	c->SetToNode(this, c->GetPos() + 1);
	command->afterCaretState = c;

	return true;
}

/**
 * Executes the create multiply formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateMultiplyFormulaNode(Command* command)
{
	command->SaveNodeState(parent);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());

	//create a multiply node and insert it into the child nodes
	FormulaNode* p = new MultiplyFormulaNode(this, wnd);
	InsertChild(p, c->GetPos());
	c->SetToNode(this, c->GetPos() + 1);
	command->afterCaretState = c;

	return true;
}

/**
 * Executes the create power formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreatePowerFormulaNode(Command* command)
{
	command->SaveNodeState(parent);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	//create a power node, insert current node into it and insert the result into the parent
	PowerFormulaNode* p = new PowerFormulaNode(this, wnd);
	if (ChildrenCount() <= pos)
		InsertChild(new EmptyFormulaNode(this), pos);
	p->base->MoveChild((*this)[pos], 0);
	FormulaNode* n = new EmptyFormulaNode(p->exponent);
	p->exponent->AddChild(n);
	InsertChild(p, pos);
	
	c->SetToNode(p->exponent, 0);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create square root formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateSquareRootFormulaNode(Command* command)
{
	command->SaveNodeState(parent);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	//create a square root node, insert current node into it and insert the result into the parent
	FormulaNode* d = new SquareRootFormulaNode(this, wnd);
	FormulaNode* expr = d->GetExpression(0);
	if (ChildrenCount() <= pos)
		InsertChild(new EmptyFormulaNode(this), pos);
	expr->MoveChild((*this)[pos], 0);
	InsertChild(d, pos);
	
	c->SetToNode(expr, 0);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create division formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateDivisionFormulaNode(Command* command)
{
	return false;
}

/**
 * Executes the create a braces formula node operation with a left brace.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateLeftBraceFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* node = c->GetNode();
	int pos = c->GetPos();
	
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

	if (parent->parent != NULL)
		command->SaveNodeState(parent->parent);
	else
		command->SaveNodeState(parent);

	//create a braces node, insert the nodes which are located to the right in the collection, into it and insert the result into the parent
	FormulaNode* d = new BracesFormulaNode(this, wnd, true, false);
	FormulaNode* expr = d->GetExpression(0);
	for (int i = pos; i < ChildrenCount();)
		expr->MoveChild((*this)[pos], expr->ChildrenCount());
	InsertChild(d, pos);

	c->SetToNode(expr, 0);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create a braces formula node operation with a right shape.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateRightBraceFormulaNode(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	FormulaNode* node = c->GetNode();
	
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
	
	//create a braces node, insert the nodes which are located to the left in the collection, into it and insert the result into the parent
	FormulaNode* d = new BracesFormulaNode(this, wnd, false, true);
	FormulaNode* expr = d->GetExpression(0);
	for (int i = pos - 1; i >= 0; --i)
		expr->MoveChild((*this)[i], 0);
	InsertChild(d, 0);

	c->SetToNode(this, 1);
	command->afterCaretState = c;
	
	return true;
}

/**
 * Executes the create equation formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateEquationFormulaNode(Command* command)
{
	//pass the request to the root node
	if (parent)
		return parent->DoCreateEquationFormulaNode(command);
	return false;
}

/**
 * Undo create equation formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreateEquationFormulaNode(Command* command)
{
	if (parent)
		return parent->UndoCreateEquationFormulaNode(command);
	return false;
}

void FormulaNode::MakeContextMenu(QMenu* menu)
{
	QAction* a;
	
	if (!MenuContainsAction(menu, tr("Cut")))
	{	
		a = new QAction(tr("Cut"), this);
		a->setShortcuts(QKeySequence::Cut);
		connect(a, SIGNAL(triggered()), this, SLOT(OnCut()));
		menu->addAction(a);
	}

	if (!MenuContainsAction(menu, tr("Copy")))
	{	
		a = new QAction(tr("Copy"), this);
		a->setShortcuts(QKeySequence::Copy);
		connect(a, SIGNAL(triggered()), this, SLOT(OnCopy()));
		menu->addAction(a);
	}

	if (!MenuContainsAction(menu, tr("Paste")))
	{	
		a = new QAction(tr("Paste"), this);
		a->setShortcuts(QKeySequence::Paste);
		connect(a, SIGNAL(triggered()), this, SLOT(OnPaste()));
		menu->addAction(a);
	}

	if (parent)	
		parent->MakeContextMenu(menu);
}

bool FormulaNode::MenuContainsAction(QMenu* menu, QString action)
{
	QList<QAction*> actions = menu->actions();
	
	for (QList<QAction*>::const_iterator iter = actions.begin(); iter != actions.end(); ++iter)
	{
		if ((*iter)->text() == action)
			return true;
	}
	
	return false;
}

/**
 * Executes the copy context menu action.
 */
void FormulaNode::OnCopy()
{
}

/**
 * Executes the paste context menu action.
 */
void FormulaNode::OnPaste()
{
}

/**
 * Executes the cut context menu action.
 */
void FormulaNode::OnCut()
{
}
