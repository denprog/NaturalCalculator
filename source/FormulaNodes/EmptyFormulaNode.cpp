#include "EmptyFormulaNode.h"
#include <QFontMetrics>

EmptyFormulaNode::EmptyFormulaNode(FormulaNode* _parent) : ShapeFormulaNode(_parent, _parent->wnd)
{
#ifdef _DEBUG
	name = "EmptyFormulaNode";
#endif
}

EmptyFormulaNode::~EmptyFormulaNode()
{
}
	
void EmptyFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	qreal h = r.height();
	qreal p = h / 10;
	baseline = font.pointSize() / 2;

	ClearShapes();
	
	AddRect(p, p, h / 2 - 2 * p, h - 2 * p, QColor("black"));
	
	boundingRect.setCoords(0, 0, h / 2, h);
}

void EmptyFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	boundingRect.setCoords(0, 0, r.height() / 2, r.height());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

FormulaNode* EmptyFormulaNode::Clone()
{
	return new EmptyFormulaNode(parent);
}

SharedCaretState EmptyFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	return SharedCaretState();
}
