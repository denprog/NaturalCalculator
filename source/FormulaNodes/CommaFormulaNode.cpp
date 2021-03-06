#include "CommaFormulaNode.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
CommaFormulaNode::CommaFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : TextShapeFormulaNode(_parent, wnd, ",")
{
	type = COMMA_NODE;
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
std::string CommaFormulaNode::ParseStructure()
{
	return ",";
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

