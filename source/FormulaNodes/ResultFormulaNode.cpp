#include "ResultFormulaNode.h"
#include "TextFormulaNode.h"
#include "MinusFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "../Main/FormulaWnd.h"

ResultFormulaNode::ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd)
{
}

ResultFormulaNode::~ResultFormulaNode()
{
}

void ResultFormulaNode::Remake()
{
	ParserExpression expr;
	
	if (wnd->parserThread->GetSolvedExpression(parserExpression.pos, expr))
	{
		//make a result node
		if (expr.exception.id == None)
		{
			childNodes->Clear();
			
			try
			{
				Real res = boost::get<BigNumbersParser::Real>(expr.result);
				
				bool mantissaSign, exponentSign;
				string mantissa, exponent;
				res.ToString(3, expr.precision, mantissaSign, mantissa, exponentSign, exponent);

				if (mantissaSign)
					AddChild(new MinusFormulaNode(this, wnd));
				TextFormulaNode* t = new TextFormulaNode(this);
				AddChild(t);
				t->SetText(mantissa.c_str());
			}
			catch (boost::bad_get)
			{
			}
			
			try
			{
				Integer res = boost::get<BigNumbersParser::Integer>(expr.result);
				res.ToString();
			}
			catch (boost::bad_get)
			{
			}
		}
		else
		{
		}
	}
	else
	{
		//make a waiting node
		if (childNodes->Count() != 1 || !dynamic_cast<TextFormulaNode*>((*this)[0]))
		{
			childNodes->Clear();
			TextFormulaNode* t = new TextFormulaNode(this);
			AddChild(t);
		}
		((TextFormulaNode*)(*this)[0])->SetText("~");
	}
	
	GroupFormulaNode::Remake();
}

void ResultFormulaNode::SetExpression(ParserExpression& expr)
{
	if (expr.expression != parserExpression.expression)
	{
		parserExpression = expr;
		//solve the expression
		wnd->parserThread->AddExpression(parserExpression);
		Remake();
	}
}
