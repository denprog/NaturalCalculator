#include "PlusFormulaNode.h"
#include "../Main/Settings.h"
#include <QFontMetrics>

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
PlusFormulaNode::PlusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
	type = PLUS_NODE;
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
	baseline = font.pointSize() / 2 + 2;

	ClearShapes();
	
	AddFillRect(w / 2 - w * 0.0025, 1, w * 0.05, w - 2, QColor("black"));
	AddFillRect(1, w / 2 - w * 0.0025, w - 2, w * 0.05, QColor("black"));
	
	//the shape, that wides the node's bounds for getting mouse events
	AddFillRect(0, 0, w, w, QColor("white"), 0);
	
	boundingRect.setCoords(0, 0, w, w);
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
	expr.Add(std::string("+"), this);
}

#ifdef TEST
void PlusFormulaNode::ParseStructure(QString& res)
{
	res += "+";
}
#endif

bool PlusFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '+')
	{
		parent->AddChild(new PlusFormulaNode(parent, parent->wnd));
		++begin;
		return true;
	}
	
	return false;
}

std::string PlusFormulaNode::ToString()
{
	return "+";
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* PlusFormulaNode::Clone(FormulaNode* p)
{
	return new PlusFormulaNode(p, wnd);
}
