#include "MinusFormulaNode.h"

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The formula window.
 */
MinusFormulaNode::MinusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : TextShapeFormulaNode(_parent, wnd, QString::fromUtf8("−"))
{
	type = MINUS_NODE;
}
	
bool MinusFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '-')
	{
		parent->AddChild(new MinusFormulaNode(parent, parent->wnd));
		++begin;
		return true;
	}
	
	return false;
}

void MinusFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("-"), this);
}

#ifdef TEST
std::string MinusFormulaNode::ParseStructure()
{
	return "-";
}
#endif
	
std::string MinusFormulaNode::ToString()
{
	return "-";
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
