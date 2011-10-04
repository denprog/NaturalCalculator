#include "MultiplyFormulaNode.h"
#include <QFontMetrics>

MultiplyFormulaNode::MultiplyFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "MultiplyFormulaNode";
#endif
}

MultiplyFormulaNode::~MultiplyFormulaNode()
{
}

void MultiplyFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	int w = r.width();
	baseline = font.pointSize() / 2;

	ClearShapes();
	
	AddFillCircle(w / 2, w / 2, w / 3, QColor("black"));
	
	//UpdateBoundingRect();
	boundingRect.setCoords(0, 0, w, w);
}

void MultiplyFormulaNode::Parse(ParserString& expr)
{
	expr.Add("*", this);
}

void MultiplyFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	boundingRect.setCoords(0, 0, r.width(), r.width());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

FormulaNode* MultiplyFormulaNode::Clone()
{
	return new MultiplyFormulaNode(parent, wnd);
}
