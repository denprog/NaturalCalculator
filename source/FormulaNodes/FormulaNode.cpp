#include "FormulaNode.h"
#include "PlusFormulaNode.h"
#include "TextFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "../Main/FormulaWnd.h"

//#if defined(WIN32) && defined(_DEBUG)
//#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#define new DEBUG_NEW
//#endif

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

void FormulaNode::AddChild(FormulaNode* node)
{
	childNodes->Add(node);
	node->item->setParentItem(item);
	node->parent = this;
}

void FormulaNode::InsertChild(FormulaNode* node, int pos)
{
	childNodes->Insert(node, pos);
	node->item->setParentItem(item);
	node->parent = this;
}

void FormulaNode::MoveChild(FormulaNode* node, int pos)
{
	int p = node->parent->GetChildPos(node);
	node->parent->childNodes->Remove(p);
	//node->parent->RemoveChild(p);
	InsertChild(node, pos);
}

void FormulaNode::RemoveChild(int pos)
{
	(*this)[pos]->item->setParentItem(NULL);
	delete (*this)[pos];
	childNodes->Remove(pos);
}

FormulaNode* FormulaNode::Clone()
{
	return new FormulaNode(parent, wnd);
}

FormulaNode* FormulaNode::operator[](int pos)
{
	return (*childNodes)[pos];
}

void FormulaNode::Remake()
{
	childNodes->Remake();
}

void FormulaNode::Update()
{
}

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

FormulaNodeLevel FormulaNode::GetLesserLevel()
{
	if (level == STILL_LESS_LEVEL)
		return STILL_LESS_LEVEL;
	return (FormulaNodeLevel)(level + 1);
}

FormulaNodeLevel FormulaNode::GetGreaterLevel()
{
	if (level == NORMAL_LEVEL)
		return NORMAL_LEVEL;
	return (FormulaNodeLevel)(level - 1);
}

void FormulaNode::SetLevel(FormulaNodeLevel _level)
{
	for (int i = 0; i < childNodes->Count(); ++i)
		(*childNodes)[i]->SetLevel(_level);
	level = _level;
}

void FormulaNode::Move(int x, int y)
{
	item->setPos(x, y);
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

void FormulaNode::SetSize()
{
}

void FormulaNode::GetHierarchyPos(HierarchyPos& positions)
{
	FormulaNode* n = this;
	while (n->parent)
	{
		positions.push_back(n->parent->GetChildPos(n));
		n = n->parent;
	}
}

void FormulaNode::Render()
{
	childNodes->Render();
}

void FormulaNode::Parse(ParserString& expr)
{
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->Parse(expr);
	}
}

int FormulaNode::GetChildPos(FormulaNode* node)
{
	return childNodes->GetPos(node);
}

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

QRectF FormulaNode::GetDocumentBounds()
{
	return parent->GetDocumentPosBounds(parent->GetChildPos(this));
}

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

bool FormulaNode::IsEmptySymbol()
{
	return childNodes->Count() == 1 && dynamic_cast<EmptyFormulaNode*>((*this)[0]);
}

SharedCaretState FormulaNode::GetFirstPosition()
{
	if (childNodes->Count() > 0)
		return childNodes->GetFirst()->GetFirstPosition();
	return SharedCaretState();
}

SharedCaretState FormulaNode::GetLastPosition()
{
	if (childNodes->Count() > 0)
		return childNodes->GetLast()->GetLastPosition();
	return SharedCaretState();
}

SharedCaretState FormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	return SharedCaretState();
}

SharedCaretState FormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	return SharedCaretState();
}

bool FormulaNode::CanSetCaret()
{
	return true;
}

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

bool FormulaNode::DoInsertLine(NodeEvent& nodeEvent)
{
	if (parent)
		return parent->DoInsertLine(nodeEvent);
	return false;
}

bool FormulaNode::UndoInsertLine(NodeEvent& nodeEvent)
{
	if (parent)
		return parent->UndoInsertLine(nodeEvent);
	return false;
}

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

//template<>
//bool FormulaNode::DoCreateNode<PlusFormulaNode>(NodeEvent& nodeEvent)
//{
//	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
//	int pos = c->GetPos();
//	FormulaNode* res = new PlusFormulaNode(this, 0, wnd);
//	
//	return true;
//}

//template<>
//bool FormulaNode::DoCreateNode<DivisionFormulaNode>(NodeEvent& nodeEvent)
//{
//	return false;
//}

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

bool FormulaNode::UndoCreatePlusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	RemoveChild(c->GetPos() - 1);
	
	return true;
}

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

bool FormulaNode::UndoCreateMinusFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	RemoveChild(c->GetPos() - 1);
	
	return true;
}

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

bool FormulaNode::UndoCreateMultiplyFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	RemoveChild(c->GetPos() - 1);
	
	return true;
}

bool FormulaNode::DoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

bool FormulaNode::UndoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

bool FormulaNode::DoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

bool FormulaNode::UndoCreateSquareRootFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

bool FormulaNode::DoCreateDivisionFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

bool FormulaNode::UndoCreateDivisionFormulaNode(NodeEvent& nodeEvent)
{
	return false;
}

bool FormulaNode::DoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	//pass the request to the root node
	if (parent)
		return parent->DoCreateEquationFormulaNode(nodeEvent);
	return false;
}

bool FormulaNode::UndoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	if (parent)
		return parent->UndoCreateEquationFormulaNode(nodeEvent);
	return false;
}
