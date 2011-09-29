#include "PlusFormulaNode.h"
#include <QFontMetrics>

PlusFormulaNode::PlusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "PlusFormulaNode";
#endif
}

PlusFormulaNode::~PlusFormulaNode()
{
}

void PlusFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	qreal w = r.width();
	qreal h = w;
	baseline = font.pointSize() / 2;

	ClearShapes();
	
	AddFillRect(w / 2, 1, 1, h - 2, QColor("black"));
	AddFillRect(1, h / 2, w - 2, 1, QColor("black"));
	
	boundingRect.setCoords(0, 0, w, h);
}

void PlusFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(NORMAL_LEVEL);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	boundingRect.setCoords(0, 0, r.width(), r.width());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

void PlusFormulaNode::Parse(ParserExpression& expr)
{
	expr.Add("+", this);
}

FormulaNode* PlusFormulaNode::Clone()
{
	return new PlusFormulaNode(parent, wnd);
}
