#include "EquationFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"
#include <QFontMetrics>
#include "../Main/FormulaWnd.h"

EquationFormulaNode::EquationFormulaNode()
{
}

EquationFormulaNode::EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();
	resNode = NULL;

#ifdef _DEBUG
	name = "EquationFormulaNode";
#endif
}

EquationFormulaNode::~EquationFormulaNode()
{
}

void EquationFormulaNode::Remake()
{
	childNodes->Remake();
	
	shape->ClearShapes();
	
	if (childNodes->Count() > 0)
	{
		//get an expression to solve
		//ParserExpression expr(this, wnd->settings->value("real/precision", 8).toInt());
		//left->Parse(expr);
		//resNode->SetExpression(expr);

		if (!resNode)
		{
			resNode = new ResultFormulaNode(this, wnd);
			AddChild(resNode);
			
			AutoParserExpression expr1(this, wnd->settings->value("auto/precision", 8).toInt(), wnd->settings->value("auto/exp", 3).toInt());
			resNode->AddResultNode(ParserExpressionVariant(expr1));
			
			RealParserExpression expr2(this, wnd->settings->value("auto/precision", 8).toInt(), wnd->settings->value("auto/exp", 3).toInt());
			resNode->AddResultNode(ParserExpressionVariant(expr2));

			IntegerParserExpression expr3(this, DECIMAL_NOTATION);
			resNode->AddResultNode(ParserExpressionVariant(expr3));

			RationalParserExpression expr4(this, IMPROPER_FRACTION);
			resNode->AddResultNode(ParserExpressionVariant(expr4));
		}
		
		FormulaNode* left = (*this)[0];
		ParserString expr;
		left->Parse(expr);
		resNode->SetExpression(expr);

		int cx = left->boundingRect.width();
		int cy = max(left->baseline, resNode->baseline);
		baseline = cy;
		
		QFont font = settings->GetTextFormulaNodeFont(level);
		QFontMetrics m(font);
		QRect r = m.boundingRect("=");

		int w = r.width();
		int h = r.height();

		left->Move(0, baseline - left->baseline);
		shape->AddFillRect(0, h * 0.3, w * 0.8, h * 0.1, QColor("black"));
		shape->AddFillRect(0, h * 0.55, w * 0.8, h * 0.1, QColor("black"));
		shape->Move(cx + w * 0.1, cy - h / 2);
		resNode->Move(cx + w, baseline - resNode->baseline);

		shape->boundingRect.setCoords(0, 0, w, h);

		UpdateBoundingRect();
	}
}

void EquationFormulaNode::UpdateBoundingRect()
{
	CompoundFormulaNode::UpdateBoundingRect();

	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("=");
	shape->boundingRect.setWidth(r.width());
	shape->boundingRect.setHeight(r.height());
}

FormulaNode* EquationFormulaNode::Clone()
{
	return new EquationFormulaNode(parent, wnd);
}
