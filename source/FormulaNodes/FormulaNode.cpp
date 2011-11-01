#include "FormulaNode.h"
#include "PlusFormulaNode.h"
#include "TextFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "PowerFormulaNode.h"
#include "SquareRootFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "../Main/FormulaWnd.h"
#include "../Util/QRectEx.h"

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
	
#ifdef _DEBUG
	name = "FormulaNode";
#endif
}

/**
 * Destructor.
 */
FormulaNode::~FormulaNode()
{
	if (command)
	{
		if (command->ContainsParam(this, "node"))
			delete any_cast<FormulaNode*>(command->GetParam(this, "node"));
	}
	
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

/**
 * Makes a deep copy of this object.
 * @return The copy of this object.
 */
FormulaNode* FormulaNode::Clone()
{
	return new FormulaNode(parent, wnd);
}

/**
 * [] casting operator.
 * @param pos The position.
 * @return The child node at the position.
 */
FormulaNode* FormulaNode::operator[](int pos)
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
void FormulaNode::GetHierarchyPos(HierarchyPos& positions)
{
	FormulaNode* n = this;
	while (n->parent)
	{
		positions.push_back(n->parent->GetChildPos(n));
		n = n->parent;
	}
}

/**
 * Renders this node.
 */
void FormulaNode::Render()
{
	childNodes->Render();
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

/**
 * Gets a child position of a node.
 * @param [in,out] node The node.
 * @return The child position.
 */
int FormulaNode::GetChildPos(FormulaNode* node)
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
			res = i;
	}
	
	return res;
}

/**
 * Query if this node is empty symbol.
 * @return true if empty symbol, false if not.
 */
bool FormulaNode::IsEmptySymbol()
{
	return childNodes->Count() == 1 && dynamic_cast<EmptyFormulaNode*>((*this)[0]);
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
SharedCaretState FormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	return SharedCaretState();
}

/**
 * Gets the previous position of this node.
 * @param [in,out] relativeState Relative caret state.
 * @return Empty caret state for FormulaNode object.
 */
SharedCaretState FormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	return SharedCaretState();
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
	
	QGraphicsItemGroup* g = wnd->caret->caretShape;
	
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

bool FormulaNode::DoInsertNode(NodeEvent& nodeEvent)
{
	return false;
}

bool FormulaNode::UndoInsertNode(NodeEvent& nodeEvent)
{
	return false;
}

/**
 * Executes the insert text operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoInsertText(NodeEvent& nodeEvent)
{
	Command* command = any_cast<Command*>(nodeEvent["command"]);
	QString str = any_cast<QString>(nodeEvent["text"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* n = c->GetNode();
	int pos = c->GetPos();
	
	if (pos < n->childNodes->Count() && dynamic_cast<EmptyFormulaNode*>((*n)[pos]))
	{
		//remove the empty node and store its being
		RemoveChild(pos);
		command->SetParam(this, "empty", true);
	}
	
	//insert a text node
	TextFormulaNode* t = new TextFormulaNode(this);
	t->SetText(str);
	InsertChild(t, pos);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoInsertText);
	c->SetToNode(t, 1);

	return true;
}

/**
 * Undo insert text.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoInsertText(NodeEvent& nodeEvent)
{
	Command* command = any_cast<Command*>(nodeEvent["command"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	bool empty = command->ContainsParam(this, "empty");
	int pos = c->GetNode()->parent->GetChildPos(c->GetNode());

	//remove the text node
	RemoveChild(pos);
		
	if (empty)
	{
		//restore the empty node
		InsertChild(new EmptyFormulaNode(this), pos);
	}
	
	return true;
}

/**
 * Executes the insert line operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoInsertLine(NodeEvent& nodeEvent)
{
	if (parent)
		return parent->DoInsertLine(nodeEvent);
	return false;
}

/**
 * Undo insert line.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoInsertLine(NodeEvent& nodeEvent)
{
	if (parent)
		return parent->UndoInsertLine(nodeEvent);
	return false;
}

/**
 * Executes the remove item operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoRemoveItem(NodeEvent& nodeEvent)
{
	command = any_cast<Command*>(nodeEvent["command"]);
	bool right = any_cast<bool>(nodeEvent["right"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();

	if (right)
	{
		if (pos < childNodes->Count())
		{
			//store the node
			command->SetParam(this, "node", (*this)[pos]->Clone());
			//remove the node
			RemoveChild(pos);
			c->SetToNode(this, pos);
		}
		else
			return false;
	}
	else
	{
		if (pos > 0)
		{
			//store the node
			command->SetParam(this, "node", (*this)[pos - 1]->Clone());
			//remove the node
			RemoveChild(pos - 1);
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
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoRemoveItem(NodeEvent& nodeEvent)
{
	command = any_cast<Command*>(nodeEvent["command"]);
	bool right = any_cast<bool>(nodeEvent["right"]);
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();
	
	InsertChild(any_cast<FormulaNode*>(command->GetParam(this, "node")), pos);
	command->RemoveParam(this, "node");
	
	return true;
}

/**
 * Executes the create plus formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreatePlusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a plus node and insert it into the child nodes
	FormulaNode* p = new PlusFormulaNode(this, wnd);
	InsertChild(p, c->GetPos());
	
	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreatePlusFormulaNode);
	c->SetToNode(this, c->GetPos() + 1);

	return true;
}

/**
 * Undo create plus formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreatePlusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	RemoveChild(c->GetPos() - 1);
	
	return true;
}

/**
 * Executes the create minus formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateMinusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a plus node and insert it into the child nodes
	FormulaNode* p = new MinusFormulaNode(this, wnd);
	InsertChild(p, c->GetPos());
	
	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateMinusFormulaNode);
	c->SetToNode(this, c->GetPos() + 1);

	return true;
}

/**
 * Undo create minus formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreateMinusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	RemoveChild(c->GetPos() - 1);
	
	return true;
}

/**
 * Executes the create multiply formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateMultiplyFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a plus node and insert it into the child nodes
	FormulaNode* p = new MultiplyFormulaNode(this, wnd);
	InsertChild(p, c->GetPos());
	
	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateMultiplyFormulaNode);
	c->SetToNode(this, c->GetPos() + 1);

	return true;
}

/**
 * Undo create multiply formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreateMultiplyFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	RemoveChild(c->GetPos() - 1);
	
	return true;
}

/**
 * Executes the create power formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();
	//create a power node, insert current node into it and insert the result into the parent
	FormulaNode* d = new PowerFormulaNode(this, wnd);
	FormulaNode* g = new GroupFormulaNode(d, wnd);
	g->MoveChild((*this)[pos], 0);
	d->InsertChild(g, 0);
	g = new GroupFormulaNode(d, wnd);
	FormulaNode* n = new EmptyFormulaNode(g);
	g->AddChild(n);
	d->AddChild(g);
	InsertChild(d, pos);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreatePowerFormulaNode);
	c->SetToNode(g, 0);
	
	return true;
}

/**
 * Undo create power formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	FormulaNode* p = parent->parent->parent;
	int pos = p->GetChildPos(parent->parent);
	p->MoveChild(this, pos);
	p->RemoveChild(pos + 1);
	
	return true;
}

/**
 * Executes the create square root formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	int pos = c->GetPos();
	//create a power node, insert current node into it and insert the result into the parent
	FormulaNode* d = new SquareRootFormulaNode(this, wnd);
	FormulaNode* g = new GroupFormulaNode(d, wnd);
	g->MoveChild((*this)[pos], 0);
	d->InsertChild(g, 1);
	InsertChild(d, pos);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateSquareRootFormulaNode);
	c->SetToNode(g, 0);
	
	return true;
}

/**
 * Undo create square root formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	FormulaNode* p = parent->parent->parent;
	int pos = p->GetChildPos(parent->parent);
	p->MoveChild(this, pos);
	p->RemoveChild(pos + 1);
	
	return true;
}

/**
 * Executes the create division formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateDivisionFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

/**
 * Undo create division formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreateDivisionFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

/**
 * Executes the create equation formula node operation.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::DoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	//pass the request to the root node
	if (parent)
		return parent->DoCreateEquationFormulaNode(nodeEvent);
	return false;
}

/**
 * Undo create equation formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool FormulaNode::UndoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	if (parent)
		return parent->UndoCreateEquationFormulaNode(nodeEvent);
	return false;
}
