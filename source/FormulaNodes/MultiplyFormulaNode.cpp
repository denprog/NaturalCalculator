#include "MultiplyFormulaNode.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
MultiplyFormulaNode::MultiplyFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : TextShapeFormulaNode(_parent, wnd, QString::fromUtf8("·"))
{
	type = MULTIPLY_NODE;
}

bool MultiplyFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '*')
	{
		parent->AddChild(new MultiplyFormulaNode(parent, parent->wnd));
		++begin;
		return true;
	}
	
	return false;
}

void MultiplyFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("*"), this);
}

#ifdef TEST
std::string MultiplyFormulaNode::ParseStructure()
{
	return "*";
}
#endif
	
std::string MultiplyFormulaNode::ToString()
{
	return "*";
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* MultiplyFormulaNode::Clone(FormulaNode* p)
{
	return new MultiplyFormulaNode(p, wnd);
}
