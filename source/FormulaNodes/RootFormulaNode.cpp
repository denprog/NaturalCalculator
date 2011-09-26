#include "RootFormulaNode.h"
#include "CompoundFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "EquationFormulaNode.h"

RootFormulaNode::RootFormulaNode()
{
}

RootFormulaNode::RootFormulaNode(FormulaNode* parent) : GroupFormulaNode(parent, parent->wnd), scene(parent->wnd->scene)
{
	scene->addItem(item);

#ifdef _DEBUG
	name = "RootFormulaNode";
#endif
}

RootFormulaNode::~RootFormulaNode()
{
}

FormulaNode* RootFormulaNode::Clone()
{
	RootFormulaNode* res = new RootFormulaNode(parent);
	res->childNodes->CopyFrom(*childNodes);
	
	return res;
}

SharedCaretState RootFormulaNode::GetFirstPosition()
{
	if (dynamic_cast<CompoundFormulaNode*>(childNodes->GetFirst()))
		return SharedCaretState(new CaretState(this, 0));
	SharedCaretState res = childNodes->GetFirst()->GetFirstPosition();
	if (res)
		return res;
	return SharedCaretState(new CaretState(this, 0));
}

SharedCaretState RootFormulaNode::GetLastPosition()
{
	if (dynamic_cast<EmptyFormulaNode*>(childNodes->GetLast()))
		return SharedCaretState(new CaretState(this, childNodes->Count() - 1));
	if (dynamic_cast<CompoundFormulaNode*>(childNodes->GetLast()))
		return SharedCaretState(new CaretState(this, childNodes->Count()));
	SharedCaretState res = childNodes->GetLast()->GetLastPosition();
	if (res)
		return res;
	return SharedCaretState(new CaretState(this, childNodes->Count()));
}

SharedCaretState RootFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	SharedCaretState res;
	
	if (!relativeState)
		res = GetFirstPosition();
	else
	{
		int i = -1;
		FormulaNode* node = relativeState->GetNode();
		if (!relativeState->CheckAtLast(this))
		{
			if (node == this)
			{
				i = relativeState->GetPos();
				FormulaNode* n = (*this)[i == childNodes->Count() ? i - 1 : i];
				res = n->GetNextPosition(relativeState);
				if (res)
					return res;
				if (i == childNodes->Count() - 1 && !dynamic_cast<EmptyFormulaNode*>(n))
					return SharedCaretState(new CaretState(this, i + 1));
			}
			else
				i = GetFirstLevelChildPos(node);
		}
		else if (node == this && parent)
			res = parent->GetNextPosition(relativeState);
		else
			i = GetFirstLevelChildPos(node);

		if (i != -1)
		{		
			if (i + 1 < childNodes->Count())
			{
				FormulaNode* n = (*this)[i + 1];
				res = n->GetNextPosition(relativeState);
				if (!res && n->CanSetCaret())
					res = SharedCaretState(new CaretState(this, i + 1));
			}
			else if (!IsEmptySymbol() && i == childNodes->Count() - 1 && *relativeState != *GetLastPosition())
				return SharedCaretState(new CaretState(this, i + 1));
		}
		
		if (!res && parent)
			return parent->GetNextPosition(relativeState);
	}
	
	return res;
}

SharedCaretState RootFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	SharedCaretState res;
	
	if (!relativeState)
		res = GetLastPosition();
	else
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
			return parent->GetPreviousPosition(relativeState);
	}
	
	return res;
}

bool RootFormulaNode::DoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* e = new EquationFormulaNode(this, wnd);
	AddChild(e);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateDivisionFormulaNode);
	c->SetToNode(this, GetChildPos(e));
	
	return true;
}

bool RootFormulaNode::UndoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	return true;
}
