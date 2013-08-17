#include "PlusFormulaNode.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
PlusFormulaNode::PlusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : TextShapeFormulaNode(_parent, wnd, "+")
{
	type = PLUS_NODE;
}

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

void PlusFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("+"), this);
}

#ifdef TEST
std::string PlusFormulaNode::ParseStructure()
{
	return "+";
}
#endif
	
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
