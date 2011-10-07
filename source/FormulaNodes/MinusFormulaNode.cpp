#include "MinusFormulaNode.h"
#include <QFontMetrics>

MinusFormulaNode::MinusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "MinusFormulaNode";
#endif
}

MinusFormulaNode::~MinusFormulaNode()
{
}
	
void MinusFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	qreal w = r.width();
	baseline = font.pointSize() / 2 - w / 2;

	ClearShapes();
	
	AddFillRect(w * 0.1, w / 2 - w * 0.45, w * 0.8, w * 0.1, QColor("black"));
	
	//UpdateBoundingRect();
	boundingRect.setCoords(0, 0, w, w);
}

void MinusFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	boundingRect.setCoords(0, 0, r.width(), r.width());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

void MinusFormulaNode::Parse(ParserString& expr)
{
	expr.Add("-", this);
}

FormulaNode* MinusFormulaNode::Clone()
{
	return new MinusFormulaNode(parent, wnd);
}
