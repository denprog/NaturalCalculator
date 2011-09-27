#include "EquationFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"
#include <QFontMetrics>

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
		left->Move(0, 0);
		
		int cx = left->boundingRect.width();
		int cy = left->baseline;
		
		QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
		QFontMetrics m(font);
		QRect r = m.boundingRect("=");

		int w = r.width();
		int h = r.height();
		baseline = left->baseline;

		shape->AddFillRect(0, h * 0.3, w * 0.8, h * 0.1, QColor("black"));
		shape->AddFillRect(0, h * 0.55, w * 0.8, h * 0.1, QColor("black"));
		shape->Move(cx + w * 0.1, cy - h / 2);

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
