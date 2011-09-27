#ifndef EQUATIONFORMULANODE_H
#define EQUATIONFORMULANODE_H

#include "CompoundFormulaNode.h"

class EquationFormulaNode : public CompoundFormulaNode
{
public:
	EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~EquationFormulaNode();

public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone();

private:
	ShapeFormulaNode* shape;
};

#endif
