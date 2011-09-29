#ifndef MINUSFORMULANODE_H
#define MINUSFORMULANODE_H

#include "ShapeFormulaNode.h"

class MinusFormulaNode : public ShapeFormulaNode
{
public:
	MinusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~MinusFormulaNode();
	
public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual void Parse(ParserExpression& expr);

	virtual FormulaNode* Clone();
};

#endif
