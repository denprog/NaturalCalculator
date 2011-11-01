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
	QRect r = m.boundingRect("+");
	
	qreal w = r.width() * 0.8;
	baseline = font.pointSize() / 2 + 2;

	ClearShapes();
	
	//the shape
	AddFillCircle(w / 2 - w / 10, w / 2 - w / 10, w / 5, QColor("black"));
	
	//the shape, that wides the node's bounds for getting mouse events
	AddFillRect(0, 0, w, w, QColor("white"), 0);

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
	QRect r = m.boundingRect("+");
	
	boundingRect.setCoords(0, 0, r.width() * 0.8, r.width());
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
