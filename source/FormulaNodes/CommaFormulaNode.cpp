#include "CommaFormulaNode.h"
#include "../Main/Settings.h"
#include <QFontMetrics>

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
CommaFormulaNode::CommaFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : ShapeFormulaNode(_parent, wnd)
{
	type = COMMA_NODE;
}

/**
 * Destructor.
 */
CommaFormulaNode::~CommaFormulaNode()
{
}

/**
 * Remakes this node.
 */
void CommaFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect(",");
	
	qreal w = r.width();
	baseline = font.pointSize() / 2 + 2;

	ClearShapes();
	
	//the shape, that wides the node's bounds for getting mouse events
	AddFillRect(0, 0, w, w, QColor("white"), 0);
	
	boundingRect.setCoords(0, 0, w, w);
}

/**
 * Updates the bounding rectangle.
 */
void CommaFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect(",");
	
	boundingRect.setCoords(0, 0, r.width(), r.width());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Adds the sign to the expression.
 * @param [in,out] expr The parser expression.
 */
void CommaFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string(","), this);
}

#ifdef TEST
void CommaFormulaNode::ParseStructure(QString& res)
{
	res += ",";
}
#endif

bool CommaFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == ',')
	{
		parent->AddChild(new CommaFormulaNode(parent, parent->wnd));
		++begin;
		return true;
	}
	
	return false;
}

std::string CommaFormulaNode::ToString()
{
	return ",";
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* CommaFormulaNode::Clone(FormulaNode* p)
{
	return new CommaFormulaNode(p, wnd);
}

