#include "GroupFormulaNode.h"
#include "ShapeFormulaNode.h"
#include "CompoundFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "FormulaNodesCollection.h"
#include <QGraphicsItemGroup>

GroupFormulaNode::GroupFormulaNode()
{
}

GroupFormulaNode::GroupFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : FormulaNode(_parent, wnd)
{
	item = new QGraphicsItemGroup(_parent ? _parent->item : NULL);

#ifdef _DEBUG
	name = "GroupFormulaNode";
#endif
}

GroupFormulaNode::~GroupFormulaNode()
{
}

void GroupFormulaNode::Remake()
{
	int x = 0;
	
	childNodes->Remake();

	Update();
	
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->Move(x, baseline - n->baseline);
		x += n->boundingRect.width();
	}

	UpdateBoundingRect();
}

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

//void GroupFormulaNode::UpdateBoundingRect()
//{
//	childNodes->UpdateBoundingRect();
//	boundingRect = item->boundingRect();
//	boundingRect.moveTo(item->pos().x(), item->pos().y());
//}

//void GroupFormulaNode::Move(int x, int y)
//{
//	item->setPos(x, y);
//	
//	//QRectF r = item->boundingRect();
//	//QPointF p = item->pos();
//	//boundingRect.moveTo(item->pos().x(), item->pos().y());
//}

FormulaNode* GroupFormulaNode::Clone()
{
	GroupFormulaNode* res = new GroupFormulaNode(parent, wnd);
	res->childNodes->CopyFrom(*childNodes);
	
	return res;
}

ShapeFormulaNode* GroupFormulaNode::AddShapeNode()
{
	ShapeFormulaNode* n = new ShapeFormulaNode(this, wnd);
	AddChild(n);
	return n;
}

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

SharedCaretState GroupFormulaNode::GetLastPosition()
{
	if (childNodes->Count() == 0)
		return SharedCaretState();
	
	FormulaNode* n = childNodes->GetLast();
	if (dynamic_cast<EmptyFormulaNode*>(n))
		return SharedCaretState(new CaretState(this, childNodes->Count() - 1));
	if (dynamic_cast<CompoundFormulaNode*>(n))
		return SharedCaretState(new CaretState(this, childNodes->Count()));
	
	SharedCaretState res = n->GetLastPosition();
	if (res)
		return res;
	
	if (childNodes->Count() == 1 && dynamic_cast<EmptyFormulaNode*>(childNodes->GetFirst()))
		return SharedCaretState(new CaretState(this, 0));
	
	return SharedCaretState(new CaretState(this, childNodes->Count()));
}

SharedCaretState GroupFormulaNode::GetNextPosition(SharedCaretState& relativeState)
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
					if (dynamic_cast<EmptyFormulaNode*>((*this)[relativeState->GetPos()]))
						return parent->GetNextPosition(relativeState);
					
					i = relativeState->GetPos();
					FormulaNode* n = (*this)[i == childNodes->Count() ? i - 1 : i];
					res = n->GetNextPosition(relativeState);
					if (res)
						return res;
					if (i == childNodes->Count() - 1 && !(dynamic_cast<EmptyFormulaNode*>(n)))
						return SharedCaretState(new CaretState(this, i + 1));
				}
				else
					i = GetFirstLevelChildPos(node);
			}
			else
				return parent->GetNextPosition(relativeState);
			
			if (i + 1 < childNodes->Count())
			{
				FormulaNode* n = (*this)[i + 1];
				res = n->GetNextPosition();
				if (res && n->CanSetCaret())
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

SharedCaretState GroupFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
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
