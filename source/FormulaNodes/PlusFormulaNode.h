#ifndef PLUSFORMULANODE_H
#define PLUSFORMULANODE_H

#include "ShapeFormulaNode.h"

class PlusFormulaNode : public ShapeFormulaNode
{
public:
	PlusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~PlusFormulaNode();
	
public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual void Parse(ParserExpression& expr);

	virtual FormulaNode* Clone();
};

#endif
