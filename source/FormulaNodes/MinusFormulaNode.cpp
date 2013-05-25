#include "MinusFormulaNode.h"
#include "../Main/Settings.h"
#include <QFontMetrics>

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The formula window.
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
	QRect r = m.boundingRect("+");
	
	qreal w = r.width();
	baseline = font.pointSize() / 2 + 2;

	ClearShapes();
	
	//the shape
	AddFillRect(1, w / 2 - w * 0.0025, w - 2, w * 0.05, QColor("black"));
	
	//the shape, that wides the node's bounds for getting mouse events
	AddFillRect(0, 0, w, w, QColor("white"), 0);
	
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
	QRect r = m.boundingRect("+");
	
	boundingRect.setCoords(0, 0, r.width(), r.width());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Adds the sign to the expression.
 * @param [in,out] expr The expression.
 */
void MinusFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("-"), this);
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* MinusFormulaNode::Clone(FormulaNode* p)
{
	return new MinusFormulaNode(p, wnd);
}
