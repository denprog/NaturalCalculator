#ifndef RESULTFORMULANODE_H
#define RESULTFORMULANODE_H

#include "GroupFormulaNode.h"
#include "../ParserThread/ParserExpression.h"

class ResultFormulaNode : public GroupFormulaNode
{
public:
	ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~ResultFormulaNode();

public:
	virtual void Remake();
	
public:
	void SetExpression(ParserExpression& expr);
	
private:
	int exp;
	ParserExpression parserExpression;
};

#endif
