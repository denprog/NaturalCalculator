#ifndef COMPOUNDFORMULANODE_H
#define COMPOUNDFORMULANODE_H

#include "GroupFormulaNode.h"

/**
 * Compound formula node.
 */
class CompoundFormulaNode : public GroupFormulaNode
{
public:
	CompoundFormulaNode();
	CompoundFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~CompoundFormulaNode();
	
public:
	virtual FormulaNode* Clone();

	virtual SharedCaretState GetNextPosition(SharedCaretState& relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState& relativeState = SharedCaretState());
};

#endif
