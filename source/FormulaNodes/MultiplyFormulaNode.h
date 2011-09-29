#ifndef MULTIPLYFORMULANODE_H
#define MULTIPLYFORMULANODE_H

#include "ShapeFormulaNode.h"

class MultiplyFormulaNode : public ShapeFormulaNode
{
public:
	MultiplyFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~MultiplyFormulaNode();

public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual void Parse(ParserExpression& expr);

	virtual FormulaNode* Clone();
};

#endif
