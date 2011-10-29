#include "MultiplyFormulaNode.h"
#include <QFontMetrics>

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
MultiplyFormulaNode::MultiplyFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "MultiplyFormulaNode";
#endif
}

/**
 * Destructor.
 */
MultiplyFormulaNode::~MultiplyFormulaNode()
{
}

/**
 * Remakes this node.
 */
void MultiplyFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	qreal w = r.width();
	baseline = font.pointSize() / 2 + 2;

	ClearShapes();
	
	AddFillCircle(w / 2 - w / 3, w / 2 - w / 3, w / 3, QColor("black"));
	
	//UpdateBoundingRect();
	boundingRect.setCoords(0, 0, w, w);
}

/**
 * Adds the sign to the expression.
 * @param [in,out] expr The expression.
 */
void MultiplyFormulaNode::Parse(ParserString& expr)
{
	expr.Add("*", this);
}

/**
 * Updates the bounding rectangle.
 */
void MultiplyFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	boundingRect.setCoords(0, 0, r.width(), r.width());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Makes a deep copy of this object.
 * @return A copy of this object.
 */
FormulaNode* MultiplyFormulaNode::Clone()
{
	return new MultiplyFormulaNode(parent, wnd);
}
