#include "GroupFormulaNode.h"
#include "ShapeFormulaNode.h"
#include "CompoundFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "../Main/Settings.h"
#include <QGraphicsItemGroup>

/**
 * Default constructor.
 */
GroupFormulaNode::GroupFormulaNode()
{
	type = GROUP_NODE;
}

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The formula window.
 */
GroupFormulaNode::GroupFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : FormulaNode(_parent, wnd)
{
	type = GROUP_NODE;
	item = new QGraphicsItemGroup(_parent ? _parent->item : NULL);
	
	//storing this pointer for identifying the item when getting mouse movements
	item->setData(0, qVariantFromValue((void*)this));
}

/**
 * Destructor.
 */
GroupFormulaNode::~GroupFormulaNode()
{
}

void GroupFormulaNode::Normalize()
{
	FormulaNode::Normalize();
	
	if (ChildrenCount() == 1)
	{
		FormulaNode* n = (*this)[0];
		if (n->type == GROUP_NODE)
		{
			for (int j = 0, k = 1; j < n->ChildrenCount();)
				MoveChild((*n)[j], k++);
			RemoveChild(0);
		}
	}
}

/**
 * Remakes this node.
 */
void GroupFormulaNode::Remake()
{
	int x = 0;
	
	childNodes->Remake();

	Update();
	
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->Move(x, baseline - n->baseline);
		x += n->boundingRect.width() + settings->GetValue("InterNodeSpace", level);
	}

	UpdateBoundingRect();
}

/**
 * Updates this node.
 */
void GroupFormulaNode::Update()
{
	baseline = 0;
	
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		if (n->baseline > baseline)
			baseline = n->baseline;
	}
}

/**
 * Makes a deep copy of this node.
 * @param [in] p The parent node.
 * @return null A copy of this object.
 */
FormulaNode* GroupFormulaNode::Clone(FormulaNode* p)
{
	GroupFormulaNode* res = new GroupFormulaNode(p, wnd);
	res->childNodes->CopyFrom(*childNodes, res);
	
	return res;
}

#ifdef TEST
void GroupFormulaNode::ParseStructure(QString& res)
{
	res += "g(";
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->ParseStructure(res);
	}
	res += ")";
}
#endif

bool GroupFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '(')
	{
		GroupFormulaNode* g = new GroupFormulaNode(parent, parent->wnd);
		++begin;
		
		while (begin != end)
		{
			if (*begin == ')')
			{
				++begin;
				break;
			}
			if (!FormulaNode::FromString(begin, end, g))
				break;
		}

		parent->AddChild(g);
		return true;
	}

	return false;
}

std::string GroupFormulaNode::ToString()
{
	return "(" + FormulaNode::ToString() + ")";
}

/**
 * Adds a shape node.
 * @return The shape node.
 */
ShapeFormulaNode* GroupFormulaNode::AddShapeNode()
{
	ShapeFormulaNode* n = new ShapeFormulaNode(this, wnd);
	AddChild(n);
	return n;
}

/**
 * Inserts a shape node.
 * @param pos The position.
 * @return The shape node.
 */
ShapeFormulaNode* GroupFormulaNode::InsertShapeNode(int pos)
{
	ShapeFormulaNode* n = new ShapeFormulaNode(this, wnd);
	InsertChild(n, pos);
	return n;
}

/**
 * Gets the first caret position of the node.
 * @return The first caret position.
 */
SharedCaretState GroupFormulaNode::GetFirstPosition()
{
	if (childNodes->Count() == 0)
		return SharedCaretState();
	
	FormulaNode* n = NULL;
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		n = (*this)[i];
		if (n->CanSetCaret())
			break;
	}
	
	if (!n)
		return SharedCaretState();
	
	if (dynamic_cast<CompoundFormulaNode*>(n))
		return SharedCaretState(new CaretState(this, 0));
	SharedCaretState res = n->GetFirstPosition();
	if (!res)
		return SharedCaretState(new CaretState(this, 0));
	
	return res;
}

/**
 * Gets the last caret position of the node.
 * @return The last position.
 */
SharedCaretState GroupFormulaNode::GetLastPosition()
{
	if (childNodes->Count() == 0)
		return SharedCaretState();
	
	FormulaNode* n = childNodes->GetLast();
	if (n->type == EMPTY_NODE)
		return SharedCaretState(new CaretState(this, childNodes->Count() - 1));
	if (dynamic_cast<CompoundFormulaNode*>(n))
		return SharedCaretState(new CaretState(this, childNodes->Count()));
	
	SharedCaretState res = n->GetLastPosition();
	if (res)
		return res;
	
	if (childNodes->Count() == 1 && childNodes->GetFirst()->type == EMPTY_NODE)
		return SharedCaretState(new CaretState(this, 0));
	
	return SharedCaretState(new CaretState(this, childNodes->Count()));
}

/**
 * Gets the next position of the node.
 * @param [in] relativeState Relative caret state.
 * @return The next position.
 */
SharedCaretState GroupFormulaNode::GetNextPosition(SharedCaretState relativeState)
{
	SharedCaretState res;
	
	if (!relativeState)
		res = GetFirstPosition();
	else
	{
		if (relativeState->CheckOnNode(this))
		{
			res = GetFirstPosition();
		}
		else if (relativeState->CheckInNode(this))
		{
			int i;
			FormulaNode* node = relativeState->GetNode();
			if (!relativeState->CheckAtLast(this))
			{
				if (node == this)
				{
					if ((*this)[relativeState->GetPos()]->type == EMPTY_NODE)
						return parent->GetNextPosition(relativeState);
					
					i = relativeState->GetPos();
					FormulaNode* n = (*this)[i == childNodes->Count() ? i - 1 : i];
					res = n->GetNextPosition(relativeState);
					if (res)
						return res;
					if (i == childNodes->Count() - 1 && n->type != EMPTY_NODE)
						return SharedCaretState(new CaretState(this, i + 1));
				}
				else
					i = GetFirstLevelChildPos(node);
			}
			else
				return parent->GetNextPosition(relativeState);
			
			if (i + 1 < childNodes->Count())
			{
				//check the next node
				FormulaNode* n = (*this)[i + 1];
				res = n->GetNextPosition();
				if (!res && n->CanSetCaret())
					res = SharedCaretState(new CaretState(this, i + 1));
			}
			else if (i == childNodes->Count() - 1 && *relativeState != *GetLastPosition())
				return SharedCaretState(new CaretState(this, i + 1));
			
			if (!res && parent)
				res = parent->GetNextPosition(relativeState);
		}
		else
			res = SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
	}
	
	return res;
}

/**
 * Gets the previous caret position of the node.
 * @param [in,out] relativeState Relative caret state.
 * @return The previous position.
 */
SharedCaretState GroupFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
{
	SharedCaretState res;
	
	if (!relativeState)
		res = GetLastPosition();
	else
	{
		if (relativeState->CheckOnNode(this))
			res = GetLastPosition();
		else if (relativeState->CheckInNode(this))
		{
			int i;
			FormulaNode* n;
			FormulaNode* node = relativeState->GetNode();
			if (node == this)
			{
				i = relativeState->GetPos();
				n = (*this)[i == childNodes->Count() ? i - 1 : i];
				res = n->GetPreviousPosition(relativeState);
				if (res)
					return res;
			}
			else
				i = GetFirstLevelChildPos(node);
			
			for (int pos = i - 1; pos >= 0; --pos)
			{
				n = (*this)[pos];
				res = n->GetPreviousPosition();
				if (!res && n->CanSetCaret())
					res = SharedCaretState(new CaretState(this, i - 1));
				if (res)
					break;
			}
			
			if (!res && parent)
				res = parent->GetPreviousPosition(relativeState);
		}
		else
			res = SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
	}
	
	return res;
}

/**
 * Returns the first caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState GroupFormulaNode::GetLineBegin(SharedCaretState& relativeState)
{
	if (*GetFirstPosition() == *relativeState && parent)
		return parent->GetLineBegin(relativeState);
	return GetFirstPosition();
}

/**
 * Returns the end caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState GroupFormulaNode::GetLineEnd(SharedCaretState& relativeState)
{
	if (*GetLastPosition() == *relativeState && parent)
		return parent->GetLineEnd(relativeState);
	return GetLastPosition();
}
