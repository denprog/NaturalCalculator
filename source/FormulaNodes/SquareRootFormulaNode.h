#ifndef SQUAREROOTFORMULANODE_H
#define SQUAREROOTFORMULANODE_H

#include "CompoundFormulaNode.h"
#include "ShapeFormulaNode.h"

class SquareRootFormulaNode : public CompoundFormulaNode
{
public:
	SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~SquareRootFormulaNode();
	
public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone();
	
	virtual void RenderCaret(const int pos, const int anchor);

private:
	ShapeFormulaNode* shape;
};

#endif
