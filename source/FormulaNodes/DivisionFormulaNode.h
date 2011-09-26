#ifndef DIVISIONFORMULANODE_H
#define DIVISIONFORMULANODE_H

#include "CompoundFormulaNode.h"

class DivisionFormulaNode : public CompoundFormulaNode
{
public:
	DivisionFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~DivisionFormulaNode();
	
public:
	virtual void AddChild(FormulaNode* node);
	virtual void InsertChild(FormulaNode* node, int pos);
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone();

	virtual SharedCaretState GetNextPosition(SharedCaretState& relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState& relativeState = SharedCaretState());

	virtual void RenderCaret(const int pos, const int anchor);

private:
	ShapeFormulaNode* shape;
};

#endif
