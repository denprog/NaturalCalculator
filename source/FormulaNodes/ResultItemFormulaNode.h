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

public:
	virtual void Normalize();

	virtual void MakeContextMenu(QMenu* menu);

#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent);
	virtual std::string ToString();
	
public:
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

public:
	virtual void Normalize();

	virtual void MakeContextMenu(QMenu* menu);

#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent);
	virtual std::string ToString();
	
public:
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

public:
	virtual void Normalize();

	virtual void MakeContextMenu(QMenu* menu);
	
#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent);
	virtual std::string ToString();
	
public:
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

public:
	virtual void Normalize();

	virtual void MakeContextMenu(QMenu* menu);
	
#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent);
	virtual std::string ToString();
	
public:
	FractionType fractionType;
};

#endif
