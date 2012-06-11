#include "ResultFormulaNode.h"
#include "TextFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ResultItemFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
ResultFormulaNode::ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd), expressionToSolve(0)
{
	delayTimer.setInterval(2000);
	connect(&delayTimer, SIGNAL(timeout()), this, SLOT(OnDelayTimer()));
}

/**
 * Destructor.
 */
ResultFormulaNode::~ResultFormulaNode()
{
}

void ResultFormulaNode::AddChild(FormulaNode* node)
{
	GroupFormulaNode::AddChild(node);
	((ResultItemFormulaNode*)node)->UpdateExpression();
}

/**
 * Remakes this node.
 */
void ResultFormulaNode::Remake()
{
	ParserExpression expr;

	//solve all the result's variants
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		ResultItemFormulaNode* n = (ResultItemFormulaNode*)(*childNodes)[i];
		ParserExpressionVariant& p = n->GetExpression();
		
		if (*p.GetSolved())
			continue;
		
		wnd->GetParserThread()->GetSolvedExpression(p);
		boost::apply_visitor(ResultNodeMaker(n), p.var);
	}
	
	childNodes->Remake();
	
	//place the result nodes
	if (childNodes->Count() == 1)
	{
		GroupFormulaNode::Remake();
		baseline = (*this)[0]->GetBaseline();
	}
	else
	{
		int cy = 0;
		for (int i = 0; i < childNodes->Count(); ++i)
		{
			FormulaNode* n = (*this)[i];
			n->Move(0, cy);
			cy += n->GetBoundingRect().height();
		}
		baseline = cy / 2;
	}
	
	UpdateBoundingRect();
}

/**
 * Sets an parser expression to be solved.
 * @param [in,out] expr The expression.
 */
void ResultFormulaNode::SetExpression(ParserString& expr)
{
	if (lastExpression == expr)
	{
		Remake();
		return;
	}
	
	lastExpression = expr;
	expr.expression += ";";
	
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		ResultItemFormulaNode* node = (ResultItemFormulaNode*)(*childNodes)[i];
		expressionToSolve = &node->GetExpression();

		if (expr.expression != expressionToSolve->GetExpression()->expression)
		{
			*(expressionToSolve->GetExpression()) = expr;
			*(expressionToSolve->GetSolved()) = false;

			delayTimer.start();
		}
	}

	Remake();
}

/**
 * Adds an automatic result node.
 * @param precision The precision.
 * @param exp The exponent.
 */
void ResultFormulaNode::AddAutoResultNode(int realPrecision, int realExp, ExpressionNotation _notation, FractionType _fractionType)
{
	AutoResultItemFormulaNode* n = new AutoResultItemFormulaNode(this, wnd, realPrecision, realExp, _notation, _fractionType);
 	AddChild(n);
}

/**
 * Adds a real result node.
 * @param precision The precision.
 * @param exp The exponent.
 */
void ResultFormulaNode::AddRealResultNode(int precision, int exp)
{
	RealResultItemFormulaNode* n = new RealResultItemFormulaNode(this, wnd, precision, exp);
	AddChild(n);
}

/**
 * Adds an integer result node.
 * @param notation The notation.
 */
void ResultFormulaNode::AddIntegerResultNode(ExpressionNotation notation)
{
	IntegerResultItemFormulaNode* n = new IntegerResultItemFormulaNode(this, wnd, notation);
	AddChild(n);
}

/**
 * Adds a rational result node.
 * @param type The fraction type.
 */
void ResultFormulaNode::AddRationalResultNode(FractionType type)
{
	RationalResultItemFormulaNode* n = new RationalResultItemFormulaNode(this, wnd, type);
	AddChild(n);
}

void ResultFormulaNode::RemoveResultNode()
{
}

void ResultFormulaNode::OnDelayTimer()
{
	//solve the expression
	wnd->GetParserThread()->AddExpression(*expressionToSolve);
	delayTimer.stop();
}

//ResultFormulaNode::ResultNodeMaker

/**
 * Constructor of the visitor.
 * @param [in,out] _parent If non-null, the parent.
 */
ResultFormulaNode::ResultNodeMaker::ResultNodeMaker(FormulaNode* _parent) : parent(_parent)
{
}

/**
 * () casting operator of the visitor for RealParserExpression expressions.
 */
void ResultFormulaNode::ResultNodeMaker::operator()(RealParserExpression const& expr) const
{
	parent->GetChildNodes()->Clear();

	if (!expr.solved)
	{
		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText("~");
		return;
	}
	
	//make an exception node
	if (expr.exception.id != None)
	{
		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText("Error!");
		return;
	}
	
	Real res = expr.result;
	
	bool mantissaSign, exponentSign;
	string mantissa, exponent;
	res.ToString(expr.exp == 0 ? parent->GetWnd()->settings->value("real/exp", 3).toInt() : expr.exp, expr.precision, 
		mantissaSign, mantissa, exponentSign, exponent);
	
	//make a result node
	if (mantissaSign)
		parent->AddChild(new MinusFormulaNode(parent, parent->GetWnd()));
	TextFormulaNode* t = new TextFormulaNode(parent);
	parent->AddChild(t);
	t->SetText(mantissa.c_str());
	
	if (!exponent.empty())
	{
		parent->AddChild(new MultiplyFormulaNode(parent, parent->GetWnd()));
		
		PowerFormulaNode* p = new PowerFormulaNode(parent, parent->GetWnd());
		parent->AddChild(p);
		TextFormulaNode* t = new TextFormulaNode(parent);
		p->InsertChild(t, 0);
		t->SetText("10");
		
		GroupFormulaNode* g = new GroupFormulaNode(p, p->GetWnd());
		p->InsertChild(g, 2);
		if (exponentSign)
			g->InsertChild(new MinusFormulaNode(p, parent->GetWnd()), 0);
		t = new TextFormulaNode(g);
		g->AddChild(t);
		t->SetText(exponent.c_str());
	}
}

/**
 * () casting operator of the visitor for IntegerParserExpression expressions.
 */
void ResultFormulaNode::ResultNodeMaker::operator()(IntegerParserExpression const& expr) const
{
	parent->GetChildNodes()->Clear();

	if (!expr.solved)
	{
		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText("~");
		return;
	}

	//make an exception node
	if (expr.exception.id != None)
	{
		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText("Error!");
		return;
	}
	
	Integer res = expr.result;

	string num = abs(res).ToString();

	//make a result node
	if (res < 0)
		parent->AddChild(new MinusFormulaNode(parent, parent->GetWnd()));
	TextFormulaNode* t = new TextFormulaNode(parent);
	parent->AddChild(t);
	t->SetText(num.c_str());
}

/**
 * () casting operator of the visitor for RationalParserExpression expressions.
 */
void ResultFormulaNode::ResultNodeMaker::operator()(RationalParserExpression const& expr) const
{
	parent->GetChildNodes()->Clear();

	if (!expr.solved)
	{
		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText("~");
		return;
	}

	//make an exception node
	if (expr.exception.id != None)
	{
		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText("Error!");
		return;
	}
	
	Rational res = expr.result;
	Integer n = abs(res.GetNumerator()), d = abs(res.GetDenomerator());

	//make a result node
	if (res < 0)
		parent->AddChild(new MinusFormulaNode(parent, parent->GetWnd()));
	
	if (expr.fractionType == IMPROPER_FRACTION || n < d)
	{
		if (d == 1)
		{
			TextFormulaNode* t = new TextFormulaNode(parent);
			parent->AddChild(t);
			t->SetText(n.ToString().c_str());
		}
		else
		{
			DivisionFormulaNode* div = new DivisionFormulaNode(parent, parent->GetWnd());
			TextFormulaNode* t = new TextFormulaNode(parent);
			div->InsertChild(t, 0);
			t->SetText(n.ToString().c_str());
			t = new TextFormulaNode(parent);
			div->InsertChild(t, 1);
			t->SetText(d.ToString().c_str());
			parent->AddChild(div);
		}
	}
	else
	{
		Integer i = n / d;
		n %= d;

		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText(i.ToString().c_str());
		
		if (n != 0)
		{
			DivisionFormulaNode* div = new DivisionFormulaNode(parent, parent->GetWnd());
			TextFormulaNode* t = new TextFormulaNode(parent);
			div->InsertChild(t, 0);
			t->SetText(n.ToString().c_str());
			t = new TextFormulaNode(parent);
			div->InsertChild(t, 1);
			t->SetText(d.ToString().c_str());
			parent->AddChild(div);
		}
	}
}

/**
 * () casting operator of the visitor for AutoParserExpression expressions.
 */
void ResultFormulaNode::ResultNodeMaker::operator()(AutoParserExpression const& expr) const
{
	RealParserExpression realExpr;
	IntegerParserExpression intExpr;
	RationalParserExpression rationalExpr;
	
	if (((AutoParserExpression&)expr).ToReal(realExpr))
		(*this)(realExpr);
	else if (((AutoParserExpression&)expr).ToInteger(intExpr))
		(*this)(intExpr);
	else if (((AutoParserExpression&)expr).ToRational(rationalExpr))
		(*this)(rationalExpr);
	else
		assert(false);
}
