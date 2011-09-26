#include "EquationFormulaNode.h"
#include <QFontMetrics>

EquationFormulaNode::EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "EquationFormulaNode";
#endif
}

EquationFormulaNode::~EquationFormulaNode()
{
}

void EquationFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
	QFontMetrics m(font);
	QRect r = m.boundingRect("=");

	int w = r.width();
	int h = r.height();
	baseline = font.pointSize();

	ClearShapes();
	
	AddFillRect(w * 0.1, h * 0.4, w * 0.8, h * 0.1, QColor("black"));
	AddFillRect(w * 0.1, h * 0.6, w * 0.8, h * 0.1, QColor("black"));
}

void EquationFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
	QFontMetrics m(font);
	QRect r = m.boundingRect("=");
	
	boundingRect.setCoords(0, 0, r.width(), r.height());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

FormulaNode* EquationFormulaNode::Clone()
{
	return new EquationFormulaNode(parent, wnd);
}
