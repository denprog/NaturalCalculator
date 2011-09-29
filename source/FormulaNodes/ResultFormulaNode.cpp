#include "ResultFormulaNode.h"
#include "TextFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "../Main/FormulaWnd.h"

ResultFormulaNode::ResultFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FormulaNode* expressionNode) : GroupFormulaNode(_parent, wnd)
{
	expressionNode->GetHierarchyPos(expressionPos);
}

ResultFormulaNode::~ResultFormulaNode()
{
}

void ResultFormulaNode::Remake()
{
	ParserExpression expr;
	
	if (wnd->parserThread->GetSolvedExpression(expressionPos, expr))
	{
		//make a result node
		if (expr.exception.id == None)
		{
			childNodes->Clear();
			
			try
			{
				Integer res = boost::get<BigNumbersParser::Integer>(expr.result);
				res.ToString();
			}
			catch (boost::bad_get)
			{
			}

			try
			{
				Real res = boost::get<BigNumbersParser::Real>(expr.result);
				
				bool mantissaSign, exponentSign;
				string mantissa, exponent;
				res.ToString(3, expr.precision, mantissaSign, mantissa, exponentSign, exponent);

				TextFormulaNode* t = new TextFormulaNode(this);
				AddChild(t);
				t->SetText(mantissa.c_str());
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
}
