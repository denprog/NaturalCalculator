#ifndef RESULTFORMULANODE_H
#define RESULTFORMULANODE_H

#include "GroupFormulaNode.h"
#include "../ParserThread/ParserExpression.h"
#include "ResultItemFormulaNode.h"
#include <QtCore/QTimer>

/**
 * Result formula node.
 */
class ResultFormulaNode : public GroupFormulaNode
{
	Q_OBJECT
	
public:
	ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);

public:
	friend class AutoResultItemFormulaNode;
	friend class RealResultItemFormulaNode;
	friend class IntegerResultItemFormulaNode;
	friend class RationalResultItemFormulaNode;
	
public:
	virtual void Remake();
	virtual void AddChild(FormulaNode* node);
	virtual void RemoveChildNodes();
	
	void SetAutoResult(int realPrecision, int realExp, ExpressionNotation notation, FractionType fractionType);
	void SetRealResult(int precision, int exp);
	void SetIntegerResult(ExpressionNotation notation);
	void SetRationalResult(FractionType fractionType);
	
	virtual bool DoChangeParams(Command* command);
	
public:
	void SetExpression(ParserString& expr);
	
#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();

public slots:
	void OnDelayTimer();
	
private:
	ResultItemFormulaNode* resultItem;
	
	struct ResultNodeMaker : boost::static_visitor<void>
	{
		ResultNodeMaker(FormulaNode* _parent);
		
		void operator()(RealParserExpression const& expr) const;
		void operator()(IntegerParserExpression const& expr) const;
		void operator()(RationalParserExpression const& expr) const;
		void operator()(AutoParserExpression const& expr) const;
		
		FormulaNode* parent;
	};
	
	QTimer delayTimer;
	ParserExpressionVariant* expressionToSolve;
	ParserString lastExpression;
	static map<ParserExceptionCode, QString> errorMessages;
};

#endif
