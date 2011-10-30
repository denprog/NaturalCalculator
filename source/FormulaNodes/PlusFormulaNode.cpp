#include "PlusFormulaNode.h"
#include <QFontMetrics>

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
PlusFormulaNode::PlusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "PlusFormulaNode";
#endif
}

/**
 * Destructor.
 */
PlusFormulaNode::~PlusFormulaNode()
{
}

/**
 * Remakes this node.
 */
void PlusFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("+");
	
	qreal w = r.width();
	qreal h = w;
	baseline = font.pointSize() / 2 + 2;

	ClearShapes();
	
	AddFillRect(w / 2 - w * 0.0025, 1, w * 0.05, h - 2, QColor("black"));
	AddFillRect(1, h / 2 - w * 0.0025, w - 2, w * 0.05, QColor("black"));
	
	boundingRect.setCoords(0, 0, w, h);
}

/**
 * Updates the bounding rectangle.
 */
void PlusFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("+");
	
	boundingRect.setCoords(0, 0, r.width(), r.width());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Adds the sign to the expression.
 * @param [in,out] expr The parser expression.
 */
void PlusFormulaNode::Parse(ParserString& expr)
{
	expr.Add("+", this);
}

/**
 * Makes a deep copy of this object.
 * @return A copy of this object.
 */
FormulaNode* PlusFormulaNode::Clone()
{
	return new PlusFormulaNode(parent, wnd);
}
