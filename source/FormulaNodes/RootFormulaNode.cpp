#include "RootFormulaNode.h"
#include "CompoundFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "EquationFormulaNode.h"

/**
 * Default constructor.
 */
RootFormulaNode::RootFormulaNode()
{
}

/**
 * Constructor.
 * @param [in] parent The parent node.
 */
RootFormulaNode::RootFormulaNode(FormulaNode* parent) : GroupFormulaNode(parent, parent->GetWnd()), scene(parent->GetWnd()->scene)
{
	scene->addItem(item);

#ifdef _DEBUG
	name = "RootFormulaNode";
#endif
}

/**
 * Destructor.
 */
RootFormulaNode::~RootFormulaNode()
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
	if (dynamic_cast<EmptyFormulaNode*>(childNodes->GetLast()))
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

/**
 * Gets the previous caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The previous caret position.
 */
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

/**
 * Executes the create equation formula node operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool RootFormulaNode::DoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	//create a equation node and move the first level nodes in its left side
	FormulaNode* e = new EquationFormulaNode(this, wnd);
	FormulaNode* g = new GroupFormulaNode(e, wnd);
	for (int i = 0, j = 0; i < childNodes->Count(); ++j)
		g->MoveChild((*childNodes)[0], j);
	e->InsertChild(g, 0);
	AddChild(e);

	nodeEvent["undoAction"] = CommandAction(this, 0, &FormulaNode::UndoCreateEquationFormulaNode);
	c->SetToNode(e, 1);
	
	return true;
}

/**
 * Undo create equation formula node.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool RootFormulaNode::UndoCreateEquationFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* node = c->GetNode();
	int pos = GetFirstLevelChildPos(node);
	MoveChild((*(*node)[0])[0], pos);
	RemoveChild(pos + 1);
	
	return true;
}

/**
 * Undo create power formula node.
 * @param [in] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool RootFormulaNode::UndoCreatePowerFormulaNode(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* n = c->GetNode();
	int pos = GetFirstLevelChildPos(n);
	RemoveChild(pos);

	return true;
}
