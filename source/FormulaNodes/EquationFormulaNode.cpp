#include "EquationFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"
#include "ResultFormulaNode.h"
#include <QFontMetrics>
#include "../Main/FormulaWnd.h"

EquationFormulaNode::EquationFormulaNode()
{
}

EquationFormulaNode::EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();

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
		FormulaNode* left = (*this)[0];

		//get expression to solve
		ParserExpression expr(this, 10);
		left->Parse(expr);

		if (childNodes->Count() == 2)
		{
			//create a result node
			ResultFormulaNode* resNode = new ResultFormulaNode(this, wnd, this);
			AddChild(resNode);
			resNode->Remake();
		}
		
		FormulaNode* right = (*this)[2];
		
		if (expr.expression != lastExpression)
		{
			//solve the expression
			wnd->parserThread->AddExpression(expr);
			lastExpression = expr.expression;
		}
		
		int cx = left->boundingRect.width();
		int cy = max(left->baseline, right->baseline);
		baseline = cy;
		
		QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
		QFontMetrics m(font);
		QRect r = m.boundingRect("=");

		int w = r.width();
		int h = r.height();

		left->Move(0, baseline - left->baseline);
		shape->AddFillRect(0, h * 0.3, w * 0.8, h * 0.1, QColor("black"));
		shape->AddFillRect(0, h * 0.55, w * 0.8, h * 0.1, QColor("black"));
		shape->Move(cx + w * 0.1, cy - h / 2);
		right->Move(cx + w, baseline - right->baseline);

		shape->boundingRect.setCoords(0, 0, w, h);

		UpdateBoundingRect();
	}
}

void EquationFormulaNode::UpdateBoundingRect()
{
	CompoundFormulaNode::UpdateBoundingRect();

	QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
	QFontMetrics m(font);
	QRect r = m.boundingRect("=");
	shape->boundingRect.setWidth(r.width());
	shape->boundingRect.setHeight(r.height());
}

FormulaNode* EquationFormulaNode::Clone()
{
	return new EquationFormulaNode(parent, wnd);
}
