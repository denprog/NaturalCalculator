#include "CompoundFormulaNode.h"
#include "FormulaNodesCollection.h"

/**
 * Default constructor.
 */
CompoundFormulaNode::CompoundFormulaNode()
{
	type = COMPOUND_NODE;
}

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
CompoundFormulaNode::CompoundFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd)
{
	type = COMPOUND_NODE;
}

/**
 * Destructor.
 */
CompoundFormulaNode::~CompoundFormulaNode()
{
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* CompoundFormulaNode::Clone(FormulaNode* p)
{
	return new CompoundFormulaNode(p, wnd);
}

/**
 * Gets the next caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The next caret position.
 */
SharedCaretState CompoundFormulaNode::GetNextPosition(SharedCaretState relativeState)
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
					if (i + 1 >= childNodes->Count())
						break;
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

/**
 * Gets the previous caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The previous caret position.
 */
SharedCaretState CompoundFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
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
			
			if (i - 1 >= 0)
			{
				n = (*this)[i - 1];
				res = n->GetPreviousPosition();
				while (!res && !n->CanSetCaret())
				{
					if (i - 1 < 0)
						break;
					--i;
					n = (*this)[i - 1];
					res = n->GetPreviousPosition();
				}
				if (!res && n->CanSetCaret())
					res = SharedCaretState(new CaretState(this, i - 1));
			}
			
			if (!res)
				res = SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
		}
	}
	
	return res;
}

/**
 * Returns the first caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState CompoundFormulaNode::GetLineBegin(SharedCaretState& relativeState)
{
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		SharedCaretState p = (*this)[i]->GetFirstPosition();
		if (p && *p == *relativeState)
			return parent->GetLineBegin(relativeState);
	}
	return GroupFormulaNode::GetLineBegin(relativeState);
}

/**
 * Returns the end caret position of the line.
 * @param [in,out] relativeState Relative caret state.
 * @return Result caret state.
 */
SharedCaretState CompoundFormulaNode::GetLineEnd(SharedCaretState& relativeState)
{
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		SharedCaretState p = (*this)[i]->GetLastPosition();
		if (p && *p == *relativeState)
			return parent->GetLineEnd(relativeState);
	}
	return GroupFormulaNode::GetLineEnd(relativeState);
}
