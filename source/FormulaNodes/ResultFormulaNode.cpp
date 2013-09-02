#include "ResultFormulaNode.h"
#include "TextFormulaNode.h"
#include "MinusFormulaNode.h"
#include "MultiplyFormulaNode.h"
#include "DivisionFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ResultItemFormulaNode.h"
#include "../Main/FormulaWnd.h"
#include "../ParserThread/ParserExpression.h"
#include <boost/assign/list_of.hpp>

map<ParserExceptionCode, QString> ResultFormulaNode::errorMessages = boost::assign::map_list_of
	(BigNumbersParser::SyntaxError, tr("Syntax error"))
	(BigNumbersParser::WrongArgumentsCount, tr("Wrong arguments count"))
	(BigNumbersParser::UnknownIdentifier, tr("Unknown identifier"))
	(BigNumbersParser::ExpressionExpected, tr("Expression expected"))
	(BigNumbersParser::DivisionByZero, tr("Division by zero"))
	(BigNumbersParser::Overflow, tr("Overflow"))
	(BigNumbersParser::ArgumentIsOver, tr("Argument is over"))
	(BigNumbersParser::ConversionDoesNotFit, tr("Conversion does not fit"));

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
ResultFormulaNode::ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd), expressionToSolve(0)
{
	type = RESULT_NODE;
	delayTimer.setInterval(2000);
	connect(&delayTimer, SIGNAL(timeout()), this, SLOT(OnDelayTimer()));
	resultItem = new AutoResultItemFormulaNode(this, wnd, wnd->settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt(), 
		wnd->settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt(), 
		(ExpressionNotation)wnd->settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt(), 
		(FractionType)wnd->settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt());
	AddChild(resultItem);
}

/**
 * Remakes this node.
 */
void ResultFormulaNode::Remake()
{
	if (resultItem == NULL)
		return;
	
	SetCanInsert(false);
	SetCanRemove(false);
	
	ParserExpressionVariant& p = resultItem->GetExpression();
	if (*p.GetSolved())
		return;
	
	wnd->parserThread->GetSolvedExpression(p);
	boost::apply_visitor(ResultNodeMaker(resultItem), p.var);
	
	GroupFormulaNode::Remake();
	baseline = resultItem->baseline;
	
	UpdateBoundingRect();
}

void ResultFormulaNode::AddChild(FormulaNode* node)
{
	RemoveChildNodes();
	GroupFormulaNode::AddChild(node);
	resultItem = (ResultItemFormulaNode*)node;
}

void ResultFormulaNode::RemoveChildNodes()
{
	GroupFormulaNode::RemoveChildNodes();
	resultItem = NULL;
}

void ResultFormulaNode::SetAutoResult(int realPrecision, int realExp, ExpressionNotation notation, FractionType fractionType)
{
	NodeEvent nodeEvent;
	nodeEvent["Type"] = std::string("Auto");
	nodeEvent["RealPrecision"] = realPrecision;
	nodeEvent["RealExp"] = realExp;
	nodeEvent["Notation"] = notation;
	nodeEvent["FractionType"] = fractionType;
	wnd->commandManager.ChangeNodeParams(nodeEvent, CommandAction(SharedCaretState(new CaretState(resultItem)), &FormulaNode::DoChangeParams));
}

void ResultFormulaNode::SetRealResult(int precision, int exp)
{
	NodeEvent nodeEvent;
	nodeEvent["Type"] = std::string("Real");
	nodeEvent["Precision"] = precision;
	nodeEvent["Exp"] = exp;
	wnd->commandManager.ChangeNodeParams(nodeEvent, CommandAction(SharedCaretState(new CaretState(resultItem)), &FormulaNode::DoChangeParams));
}

void ResultFormulaNode::SetIntegerResult(ExpressionNotation notation)
{
	NodeEvent nodeEvent;
	nodeEvent["Type"] = std::string("Integer");
	nodeEvent["Notation"] = notation;
	wnd->commandManager.ChangeNodeParams(nodeEvent, CommandAction(SharedCaretState(new CaretState(resultItem)), &FormulaNode::DoChangeParams));
}

void ResultFormulaNode::SetRationalResult(FractionType fractionType)
{
	NodeEvent nodeEvent;
	nodeEvent["Type"] = std::string("Rational");
	nodeEvent["FractionType"] = fractionType;
	wnd->commandManager.ChangeNodeParams(nodeEvent, CommandAction(SharedCaretState(new CaretState(resultItem)), &FormulaNode::DoChangeParams));
}

bool ResultFormulaNode::DoChangeParams(Command* command)
{
	command->SaveNodeState(parent->parent);
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	std::string type = any_cast<std::string>(command->nodeEvent["Type"]);
	
	if (type == "Auto")
	{
		int realPrecision = any_cast<int>(command->nodeEvent["RealPrecision"]);
		int realExp = any_cast<int>(command->nodeEvent["RealExp"]);
		FractionType fractionType = any_cast<FractionType>(command->nodeEvent["FractionType"]);
		ExpressionNotation notation = any_cast<ExpressionNotation>(command->nodeEvent["Notation"]);
		
		if (resultItem->type == AUTO_RESULT_ITEM_NODE)
		{
			AutoResultItemFormulaNode* node = (AutoResultItemFormulaNode*)resultItem;
			if (node->realPrecision == realPrecision && node->realExp == realExp && node->fractionType == fractionType && node->notation == notation)
				return false;
		}
		
		resultItem = new AutoResultItemFormulaNode(this, wnd, realPrecision, realExp, notation, fractionType);
	}
	else if (type == "Real")
	{
		int precision = any_cast<int>(command->nodeEvent["Precision"]);
		int exp = any_cast<int>(command->nodeEvent["Exp"]);

		if (resultItem->type == REAL_RESULT_ITEM_NODE)
		{
			RealResultItemFormulaNode* node = (RealResultItemFormulaNode*)resultItem;
			if (node->precision == precision && node->exp == exp)
				return false;
		}
		
		resultItem = new RealResultItemFormulaNode(this, wnd, precision, exp);
	}
	else if (type == "Integer")
	{
		ExpressionNotation notation = any_cast<ExpressionNotation>(command->nodeEvent["Notation"]);

		if (resultItem->type == INTEGER_RESULT_ITEM_NODE)
		{
			IntegerResultItemFormulaNode* node = (IntegerResultItemFormulaNode*)resultItem;
			if (node->notation == notation)
				return false;
		}
		
		resultItem = new IntegerResultItemFormulaNode(this, wnd, notation);
	}
	else if (type == "Rational")
	{
		FractionType fractionType = any_cast<FractionType>(command->nodeEvent["FractionType"]);

		if (resultItem->type == RATIONAL_RESULT_ITEM_NODE)
		{
			RationalResultItemFormulaNode* node = (RationalResultItemFormulaNode*)resultItem;
			if (node->fractionType == fractionType)
				return false;
		}
		
		resultItem = new RationalResultItemFormulaNode(this, wnd, fractionType);
	}
	else
	{
		assert(false);
	}
	
	lastExpression.expression = "";
	resultItem->Normalize();
	ReplaceChild(resultItem, 0);
	c->SetToNode(parent->parent, 1);
	
	command->afterCaretState = c;
	
	return true;
}

/**
 * Sets an parser expression to be solved.
 * @param [in,out] expr The expression.
 */
void ResultFormulaNode::SetExpression(ParserString& expr)
{
	if (lastExpression == expr && !expr.expression.empty())
	{
		Remake();
		return;
	}
	
	lastExpression = expr;
	expr.expression += ";";
	
	expressionToSolve = &resultItem->GetExpression();

	if (expr.expression != expressionToSolve->GetExpression()->expression)
	{
		*(expressionToSolve->GetExpression()) = expr;
		*(expressionToSolve->GetSolved()) = false;
		delayTimer.start();
	}

	Remake();
}

#ifdef TEST
std::string ResultFormulaNode::ParseStructure()
{
	return "$res(" + GroupFormulaNode::ParseStructure() + ")";
}
#endif

bool ResultFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	std::string::iterator i = begin;
	
	if (FindSubstring(begin, end, "$res"))
	{
		ResultFormulaNode* r = new ResultFormulaNode(parent, parent->wnd);

		i += 4;
		if (!FormulaNode::FromNestedString(i, end, r))
		{
			delete r;
			return false;
		}

		parent->AddChild(r);
		begin = i;
		return true;
	}
	
	return false;
}

std::string ResultFormulaNode::ToString()
{
	return "$res" + GroupFormulaNode::ToString();
}

void ResultFormulaNode::OnDelayTimer()
{
	//solve the expression
	wnd->parserThread->AddExpression(*expressionToSolve);
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
	parent->childNodes->Clear();
	
	if (!expr.solved)
	{
		UpdateWaitingSymbol();
		return;
	}

	//make an exception node
	if (expr.exception.id != None)
	{
		MakeExceptionNode(expr.expression, expr.exception);
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
	
	if (!expr.solved)
	{
		UpdateWaitingSymbol();
		return;
	}

	//make an exception node
	if (expr.exception.id != None)
	{
		MakeExceptionNode(expr.expression, expr.exception);
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
	
	if (!expr.solved)
	{
		UpdateWaitingSymbol();
		return;
	}

	//make an exception node
	if (expr.exception.id != None)
	{
		MakeExceptionNode(expr.expression, expr.exception);
		return;
	}
	
	Rational res = expr.result;
	Integer n = abs(res.GetNumerator()), d = abs(res.GetDenomerator());

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
			t->SetText(n.ToString().c_str());
			div->dividend->AddChild(t);
			t = new TextFormulaNode(parent);
			t->SetText(d.ToString().c_str());
			div->divisor->AddChild(t);
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
			t->SetText(n.ToString().c_str());
			div->dividend->AddChild(t);
			t = new TextFormulaNode(parent);
			t->SetText(d.ToString().c_str());
			div->divisor->AddChild(t);
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

void ResultFormulaNode::ResultNodeMaker::UpdateWaitingSymbol() const
{
	EquationFormulaNode* e = (EquationFormulaNode*)parent->GetParentByType(EQUATION_NODE);
	if (parent->wnd->caret->currentState->CheckInNode(parent))
	{
		parent->wnd->caret->SetToNode(e, 1);
		QCoreApplication::postEvent(parent->wnd, new QEvent((QEvent::Type)FormulaWnd::updateCaretEventId));
	}
	TextFormulaNode* t = new TextFormulaNode(parent);
	t->SetText("~");
	parent->AddChild(t);
	e->errorPos.clear();
}

void ResultFormulaNode::ResultNodeMaker::MakeExceptionNode(ParserString expression, ParserException exception) const
{
	TextFormulaNode* t = new TextFormulaNode(parent);
	parent->AddChild(t);
	t->SetText(errorMessages[exception.id]);
	
	EquationFormulaNode* e = (EquationFormulaNode*)parent->GetParentByType(EQUATION_NODE);
	assert(e);
	
	HierarchyPos errorPos;
	int j = INT_MAX;
	
	//find the shortest annotation chunk, it is the error position
	for (size_t i = 0; i < expression.annotation.size(); ++i)
	{
		ParserString::AnnotationPos p = expression.annotation[i];
		if (exception.pos >= p.pos && exception.pos <= p.pos + p.length)
		{
			if (j > p.length)
				errorPos = p.hierarchyPos;
		}
	}
	
	e->errorPos = errorPos;
}
