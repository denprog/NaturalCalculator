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
	void SetExpression(ParserString& expr);
	
	void AddResultNode(ParserExpressionVariant& expr);
	void RemoveResultNode();

private:
	//template<class Number>
	//bool MakeResultNode(ParserExpression& expr, FormulaNode* p);
	
	struct ResultNodeMaker : boost::static_visitor<void>
	{
		ResultNodeMaker(FormulaNode* _parent);
		
		void operator()(RealParserExpression const& expr) const;
		void operator()(IntegerParserExpression const& expr) const;
		void operator()(RationalParserExpression const& expr) const;
		void operator()(AutoParserExpression const& expr) const;
		
		FormulaNode* parent;
	};
		
private:
	//int exp;
	//ParserExpression parserExpression;
	//vector<ParserExpression> parserExpressions;
	vector<ParserExpressionVariant> parserExpressions;
};

#endif
