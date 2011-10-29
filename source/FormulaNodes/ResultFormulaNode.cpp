#include "ResultFormulaNode.h"
#include "TextFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "../Main/FormulaWnd.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
ResultFormulaNode::ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd)
{
}

/**
 * Destructor.
 */
ResultFormulaNode::~ResultFormulaNode()
{
}

/**
 * Sets an parser expression to be solved.
 * @param [in,out] expr The expression.
 */
void ResultFormulaNode::SetExpression(ParserString& expr)
{
	for (int i = 0; i < (int)parserExpressions.size(); ++i)
	{
		ParserExpressionVariant& p = parserExpressions[i];
		if (expr.expression != p.GetExpression()->expression)
		{
			*p.GetExpression() = expr;
			*p.GetSolved() = false;
			
			//solve the expression
			wnd->parserThread->AddExpression(p);
			Remake();
		}
	}
}

/**
 * Adds a result node.
 * @param [in,out] expr The expression.
 */
void ResultFormulaNode::AddResultNode(ParserExpressionVariant& expr)
{
	AddChild(new GroupFormulaNode(this, wnd));
	parserExpressions.push_back(expr);
}

void ResultFormulaNode::RemoveResultNode()
{
}

//template<>
//bool ResultFormulaNode::MakeResultNode<Integer>(ParserExpression& expr, FormulaNode* p)
//{
//	return true;
//}
//
//template<>
//bool ResultFormulaNode::MakeResultNode<Real>(ParserExpression& expr, FormulaNode* p)
//{
//	p->childNodes->Clear();
//
//	//make an exception node
//	if (expr.exception.id != None)
//	{
//		TextFormulaNode* t = new TextFormulaNode(this);
//		p->AddChild(t);
//		t->SetText("Error!");
//		return true;
//	}
//	
//	Real res;	
//	try
//	{
//		res = boost::get<Real>(expr.result);
//	}
//	catch (boost::bad_get)
//	{
//		return false;
//	}
//	
//	//make a result node
//	bool mantissaSign, exponentSign;
//	string mantissa, exponent;
//	res.ToString(expr.GetParam<int>("exp") == 0 ? wnd->settings->value("real/exp", 3).toInt() : expr.GetParam<int>("exp"), expr.GetParam<int>("precision"), 
//		mantissaSign, mantissa, exponentSign, exponent);
//	
//	if (mantissaSign)
//		p->AddChild(new MinusFormulaNode(this, wnd));
//	TextFormulaNode* t = new TextFormulaNode(this);
//	p->AddChild(t);
//	t->SetText(mantissa.c_str());
//	
//	return true;
//}
//
//template<>
//bool ResultFormulaNode::MakeResultNode<Rational>(ParserExpression& expr, FormulaNode* p)
//{
//	return true;
//}

/**
 * Remakes this node.
 */
void ResultFormulaNode::Remake()
{
	ParserExpression expr;

	//solve all the result's variants
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*childNodes)[i];
		//ParserExpression& p = parserExpressions[i];
		ParserExpressionVariant& p = parserExpressions[i];
		
		if (*p.GetSolved())
			continue;
		
		if (wnd->parserThread->GetSolvedExpression(p))
		{
			boost::apply_visitor(ResultNodeMaker(n), p.var);
		}
		
		//if (wnd->parserThread->GetSolvedExpression(p))
		//{
		//	switch (p.type)
		//	{
		//	case AUTO:
		//		//find actual result type and make a result node
		//		if (MakeResultNode<Real>(p, n))
		//			break;
		//		if (MakeResultNode<Integer>(p, n))
		//			break;
		//		break;
		//	case INTEGER:
		//		MakeResultNode<Integer>(p, n);
		//		break;
		//	case REAL:
		//		MakeResultNode<Real>(p, n);
		//		break;
		//	case RATIONAL:
		//		MakeResultNode<Rational>(p, n);
		//		break;
		//	}
		//	
		//	p.solved = true;
		//}
		//else
		//{
		//	//make a waiting node
		//	n->childNodes->Clear();
		//	TextFormulaNode* t = new TextFormulaNode(this);
		//	t->SetText("~");
		//	n->AddChild(t);
		//}
	}
	
	childNodes->Remake();
	
	//place the result nodes
	if (childNodes->Count() == 1)
	{
		GroupFormulaNode::Remake();
		baseline = (*this)[0]->baseline;
	}
	else
	{
		int cy = 0;
		for (int i = 0; i < childNodes->Count(); ++i)
		{
			FormulaNode* n = (*this)[i];
			n->Move(0, cy);
			cy += n->boundingRect.height();
		}
		baseline = cy / 2;
	}
	
	UpdateBoundingRect();
	
	//GroupFormulaNode::Remake();
}

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
	parent->childNodes->Clear();

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
	res.ToString(expr.exp == 0 ? parent->wnd->settings->value("real/exp", 3).toInt() : expr.exp, expr.precision, 
		mantissaSign, mantissa, exponentSign, exponent);
	
	//make a result node
	if (mantissaSign)
		parent->AddChild(new MinusFormulaNode(parent, parent->wnd));
	TextFormulaNode* t = new TextFormulaNode(parent);
	parent->AddChild(t);
	t->SetText(mantissa.c_str());
	
	if (!exponent.empty())
	{
		parent->AddChild(new MultiplyFormulaNode(parent, parent->wnd));
		
		PowerFormulaNode* p = new PowerFormulaNode(parent, parent->wnd);
		parent->AddChild(p);
		TextFormulaNode* t = new TextFormulaNode(parent);
		p->InsertChild(t, 0);
		t->SetText("10");
		
		GroupFormulaNode* g = new GroupFormulaNode(p, p->wnd);
		p->InsertChild(g, 2);
		if (exponentSign)
			g->InsertChild(new MinusFormulaNode(p, parent->wnd), 0);
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
	parent->childNodes->Clear();

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
		parent->AddChild(new MinusFormulaNode(parent, parent->wnd));
	TextFormulaNode* t = new TextFormulaNode(parent);
	parent->AddChild(t);
	t->SetText(num.c_str());
}

/**
 * () casting operator of the visitor for RationalParserExpression expressions.
 */
void ResultFormulaNode::ResultNodeMaker::operator()(RationalParserExpression const& expr) const
{
	parent->childNodes->Clear();

	//make an exception node
	if (expr.exception.id != None)
	{
		TextFormulaNode* t = new TextFormulaNode(parent);
		parent->AddChild(t);
		t->SetText("Error!");
		return;
	}
	
	Rational res = expr.result;
	Integer n = res.GetNumerator(), d = res.GetDenomerator();

	//make a result node
	if (res < 0)
		parent->AddChild(new MinusFormulaNode(parent, parent->wnd));
	
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
			DivisionFormulaNode* div = new DivisionFormulaNode(parent, parent->wnd);
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
			DivisionFormulaNode* div = new DivisionFormulaNode(parent, parent->wnd);
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
	if (((AutoParserExpression&)expr).ToReal(realExpr))
	{
		(*this)(realExpr);
		return;
	}
}
