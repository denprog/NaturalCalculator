#include "RootFormulaNode.h"
#include "CompoundFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "EquationFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Default constructor.
 */
RootFormulaNode::RootFormulaNode()
{
	type = ROOT_NODE;
}

/**
 * Constructor.
 * @param [in] parent The parent node.
 */
RootFormulaNode::RootFormulaNode(FormulaNode* parent) : GroupFormulaNode(parent, parent->wnd), scene(parent->wnd->scene)
{
	type = ROOT_NODE;
	scene->addItem(item);
}

RootFormulaNode::RootFormulaNode(FormulaWnd* wnd) : GroupFormulaNode(NULL, wnd), scene(wnd->scene)
{
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* RootFormulaNode::Clone(FormulaNode* p)
{
	RootFormulaNode* res = new RootFormulaNode(p);
	res->childNodes->CopyFrom(*childNodes, res);
	
	return res;
}

/**
 * Gets the first caret position.
 * @return The first caret position.
 */
SharedCaretState RootFormulaNode::GetFirstPosition()
{
	if (dynamic_cast<CompoundFormulaNode*>(childNodes->GetFirst()))
		return SharedCaretState(new CaretState(this, 0));
	SharedCaretState res = childNodes->GetFirst()->GetFirstPosition();
	if (res)
		return res;
	return SharedCaretState(new CaretState(this, 0));
}

/**
 * Gets the last caret position.
 * @return The last caret position.
 */
SharedCaretState RootFormulaNode::GetLastPosition()
{
	if (childNodes->GetLast()->type == EMPTY_NODE)
		return SharedCaretState(new CaretState(this, childNodes->Count() - 1));
	if (dynamic_cast<CompoundFormulaNode*>(childNodes->GetLast()))
		return SharedCaretState(new CaretState(this, childNodes->Count()));
	SharedCaretState res = childNodes->GetLast()->GetLastPosition();
	if (res)
		return res;
	return SharedCaretState(new CaretState(this, childNodes->Count()));
}

/**
 * Gets the next caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The next caret position.
 */
SharedCaretState RootFormulaNode::GetNextPosition(SharedCaretState relativeState)
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
				if (i == childNodes->Count() - 1 && n->type != EMPTY_NODE)
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

/**
 * Gets the previous caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The previous caret position.
 */
SharedCaretState RootFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
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

/**
 * Executes the create equation formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool RootFormulaNode::DoCreateEquationFormulaNode(Command* command)
{
	command->SaveNodeState(this);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	//create a equation node and move the first level nodes in its left side
	EquationFormulaNode* e = new EquationFormulaNode(this, wnd);
	for (int i = 0, j = 0; i < childNodes->Count(); ++j)
		e->left->MoveChild((*childNodes)[0], j);
	AddChild(e);
	
	c->SetToNode(e, 1);
	command->afterCaretState = c;
	
	return true;
}

RootFormulaNode* RootFormulaNode::FromString(std::string str, FormulaWnd* wnd)
{
	RootFormulaNode* res = new RootFormulaNode(wnd->documentNode);
	
	std::string::iterator begin = str.begin();
	std::string::iterator end = str.end();
	
	while (begin != str.end())
	{
		if (!FormulaNode::FromString(begin, end, res))
			++begin;
	}
	
	res->Normalize();
	
	return res;
}
