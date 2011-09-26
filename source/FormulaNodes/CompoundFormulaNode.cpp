#include "CompoundFormulaNode.h"
#include "FormulaNodesCollection.h"

CompoundFormulaNode::CompoundFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "CompoundFormulaNode";
#endif
}

CompoundFormulaNode::~CompoundFormulaNode()
{
}

FormulaNode* CompoundFormulaNode::Clone()
{
	return new CompoundFormulaNode(parent, wnd);
}

SharedCaretState CompoundFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	SharedCaretState res;
	
	if (!relativeState)
		res = SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
	else
	{
		if (relativeState->CheckOnNode(this))
			res = GetFirstPosition();
		else if (relativeState->CheckInNode(this))
		{
			int i;
			FormulaNode* n;
			FormulaNode* node = relativeState->GetNode();
			if (node == this)
			{
				i = relativeState->GetPos();
				n = (*this)[i == childNodes->Count() ? i - 1 : i];
				res = n->GetNextPosition(relativeState);
				if (res)
					return res;
			}
			else
				i = GetFirstLevelChildPos(node);
			
			if (i + 1 < childNodes->Count())
			{
				n = (*this)[i + 1];
				res = n->GetNextPosition();
				while (!res && !n->CanSetCaret())
				{
					++i;
					n = (*this)[i + 1];
					res = n->GetNextPosition();
				}
				if (!res && n->CanSetCaret())
					res = SharedCaretState(new CaretState(this, i + 1));
			}
			
			if (!res && parent)
				res = parent->GetNextPosition(relativeState);
		}
		else if (relativeState->CheckAtLast(this))
			res = parent->GetNextPosition(relativeState);
		else
			res = SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
	}
	
	return res;
}

SharedCaretState CompoundFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	SharedCaretState res;
	
	if (!relativeState)
		res = GetLastPosition();
	else
	{
		if (relativeState->CheckInNode(this))
		{
			int i;
			FormulaNode* n;
			FormulaNode* node = relativeState->GetNode();
			if (!relativeState->CheckAtLast(this))
			{
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
			}
			else
				i = childNodes->Count();
			
			for (int pos = i - 1; pos >= 0; --pos)
			{
				n = (*this)[pos];
				res = n->GetPreviousPosition();
				if (res)
					break;
			}
			
			if (!res)
				res = SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
		}
	}
	
	return res;
}
