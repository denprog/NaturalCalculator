#include "MinusFormulaNode.h"
#include <QFontMetrics>

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
MinusFormulaNode::MinusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "MinusFormulaNode";
#endif
}

/**
 * Destructor.
 */
MinusFormulaNode::~MinusFormulaNode()
{
}
	
/**
 * Remakes this node.
 */
void MinusFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("-");
	
	qreal w = r.width();
	baseline = font.pointSize() / 2 + 2;

	ClearShapes();
	
	//the shape
	AddFillRect(0, w / 2, w, w * 0.1, QColor("black"));
	
	//UpdateBoundingRect();
	boundingRect.setCoords(0, 0, w, w);
}

/**
 * Updates the bounding rectangle.
 */
void MinusFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("-");
	
	boundingRect.setCoords(0, 0, r.width(), r.height());
	boundingRect.moveTo(item->pos().x(), 0);
}

/**
 * Adds the sign to the expression.
 * @param [in,out] expr The expression.
 */
void MinusFormulaNode::Parse(ParserString& expr)
{
	expr.Add("-", this);
}

/**
 * Makes a deep copy of this object.
 * @return A copy of this object.
 */
FormulaNode* MinusFormulaNode::Clone()
{
	return new MinusFormulaNode(parent, wnd);
}
