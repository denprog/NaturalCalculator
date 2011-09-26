#ifndef EQUATIONFORMULANODE_H
#define EQUATIONFORMULANODE_H

#include "ShapeFormulaNode.h"

class EquationFormulaNode : public ShapeFormulaNode
{
public:
	EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~EquationFormulaNode();

public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone();
};

#endif
