#include "EmptyFormulaNode.h"
#include "../Main/Settings.h"
#include <QFontMetrics>

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 */
EmptyFormulaNode::EmptyFormulaNode(FormulaNode* _parent) : ShapeFormulaNode(_parent, _parent->wnd)
{
	type = EMPTY_NODE;
}

/**
 * Destructor.
 */
EmptyFormulaNode::~EmptyFormulaNode()
{
}
	
/**
 * Remakes this node.
 */
void EmptyFormulaNode::Remake()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	qreal h = r.height();
	qreal p = h / 10;
	baseline = font.pointSize();

	ClearShapes();
	
	AddRect(p, p, h / 2 - 2 * p, h - 2 * p, QColor("black"));
	
	boundingRect.setCoords(0, 0, h / 2, h);
}

/**
 * Updates the bounding rectangle.
 */
void EmptyFormulaNode::UpdateBoundingRect()
{
	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("T");
	
	boundingRect.setCoords(0, 0, r.height() / 2, r.height());
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* EmptyFormulaNode::Clone(FormulaNode* p)
{
	return new EmptyFormulaNode(p);
}

#ifdef TEST
void EmptyFormulaNode::ParseStructure(QString& res)
{
	res += "e";
}
#endif

bool EmptyFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '~')
	{
		parent->AddChild(new EmptyFormulaNode(parent));
		++begin;
		return true;
	}
	
	return false;
}

std::string EmptyFormulaNode::ToString()
{
	return "~";
}

/**
 * Gets the next caret position of the node.
 * @param [in,out] relativeState State of the relative.
 * @return The empty position.
 */
SharedCaretState EmptyFormulaNode::GetNextPosition(SharedCaretState relativeState)
{
	return SharedCaretState();
}

bool EmptyFormulaNode::IsEmptySymbol()
{
	return true;
}
