#ifndef RESULTITEMFORMULANODE_H
#define RESULTITEMFORMULANODE_H

#include "GroupFormulaNode.h"

/**
 * Result item formula node.
 */
class ResultItemFormulaNode : public GroupFormulaNode
{
	Q_OBJECT
	
public:
	ResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~ResultItemFormulaNode();

public slots:
	void OnPresentAsAutoResult();
	void OnPresentAsRealResult();
	void OnPresentAsIntegerResult();
	void OnPresentAsRationalResult();
	
public:
	ParserExpressionVariant& GetExpression()
	{
		expression.UpdatePos(this);
		return expression;
	}
	
protected:
	ParserExpressionVariant expression;
};

/**
 * Automatic result item formula node.
 */
class AutoResultItemFormulaNode : public ResultItemFormulaNode
{
	Q_OBJECT
	
public:
	AutoResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, int _precision, int _exp, ExpressionNotation _notation, FractionType _fractionType);
	virtual ~AutoResultItemFormulaNode();

public:
	virtual void Normalize();

	virtual void MakeContextMenu(QMenu* menu);

#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent);
	virtual std::string ToString();
	
private:
	int realPrecision;
	int realExp;
	ExpressionNotation notation;
	FractionType fractionType;
};

/**
 * Real result item formula node.
 */
class RealResultItemFormulaNode : public ResultItemFormulaNode
{
	Q_OBJECT

public:
	RealResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, int _precision, int _exp);
	virtual ~RealResultItemFormulaNode();

public:
	virtual void UpdateExpression();

	virtual void MakeContextMenu(QMenu* menu);

private:
	int precision;
	int exp;
};

/**
 * Integer result item formula node.
 */
class IntegerResultItemFormulaNode : public ResultItemFormulaNode
{
	Q_OBJECT

public:
	IntegerResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, ExpressionNotation _notation);
	virtual ~IntegerResultItemFormulaNode();

public:
	virtual void UpdateExpression();

	virtual void MakeContextMenu(QMenu* menu);
	
private:
	ExpressionNotation notation;
};

/**
 * Rational result item formula node.
 */
class RationalResultItemFormulaNode : public ResultItemFormulaNode
{
	Q_OBJECT

public:
	RationalResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FractionType _fractionType);
	virtual ~RationalResultItemFormulaNode();

public:
	virtual void UpdateExpression();

	virtual void MakeContextMenu(QMenu* menu);
	
private:
	FractionType fractionType;
};

#endif
