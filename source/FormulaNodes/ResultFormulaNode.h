#ifndef RESULTFORMULANODE_H
#define RESULTFORMULANODE_H

#include "GroupFormulaNode.h"
#include "../ParserThread/ParserExpression.h"

/**
 * Result formula node.
 */
class ResultFormulaNode : public GroupFormulaNode
{
public:
	ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~ResultFormulaNode();

public:
	virtual void AddChild(FormulaNode* node);
	virtual void Remake();
	
public:
	void SetExpression(ParserString& expr);
	
	void AddAutoResultNode(int realPrecision, int realExp, ExpressionNotation _notation, FractionType _fractionType);
	void AddRealResultNode(int precision, int exp);
	void AddIntegerResultNode(ExpressionNotation notation);
	void AddRationalResultNode(FractionType type);
	void RemoveResultNode();

private:
	struct ResultNodeMaker : boost::static_visitor<void>
	{
		ResultNodeMaker(FormulaNode* _parent);
		
		void operator()(RealParserExpression const& expr) const;
		void operator()(IntegerParserExpression const& expr) const;
		void operator()(RationalParserExpression const& expr) const;
		void operator()(AutoParserExpression const& expr) const;
		
		FormulaNode* parent;
	};
};

#endif
