#include "PowerFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"
#include "../Main/Settings.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
PowerFormulaNode::PowerFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	type = POWER_NODE;
	shape = AddShapeNode();

#ifdef _DEBUG
	name = "PowerFormulaNode";
#endif
}

/**
 * Destructor.
 */
PowerFormulaNode::~PowerFormulaNode()
{
}

/**
 * Remakes this node.
 */
void PowerFormulaNode::Remake()
{
	if (childNodes->Count() > 1)
	{
		FormulaNode* left = (*this)[0];
		FormulaNode* right = (*this)[2];

		right->SetLevel(GetLesserLevel());

		childNodes->Remake();
		shape->ClearShapes();

		//the shape
		shape->AddFillRect(0, 0, left->GetBoundingRect().height() / 10, left->GetBoundingRect().height(), QColor("white"));
		shape->UpdateBoundingRect();
		
		int cy = right->GetBoundingRect().height() * 0.5;
		left->Move(0, cy);
		shape->Move(left->GetBoundingRect().width() + settings->GetValue("InterSymbolSpace", level) / 2, cy);
		right->Move(shape->GetBoundingRect().right() + settings->GetValue("InterSymbolSpace", level) / 2, 0);

		UpdateBoundingRect();
		
		baseline = left->GetBaseline() + cy;
	}
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* PowerFormulaNode::Clone(FormulaNode* p)
{
	return new PowerFormulaNode(p, wnd);
}

/**
 * Parses the child nodes and adds the function name to the expression.
 * @param [in,out] expr The expression.
 */
void PowerFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("pow("), this);
	(*this)[0]->Parse(expr);
	expr.Add(std::string(","), this);
	(*this)[2]->Parse(expr);
	expr.Add(std::string(")"), this);
}

#ifdef TEST
void PowerFormulaNode::ParseStructure(QString& res)
{
	res += "pow(";
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->ParseStructure(res);
	}
	res += ")";
}
#endif
