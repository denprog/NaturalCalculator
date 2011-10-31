#include "PowerFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
PowerFormulaNode::PowerFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
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
		shape->AddFillRect(0, 0, left->boundingRect.height() / 10, left->boundingRect.height(), QColor("white"));
		shape->UpdateBoundingRect();
		
		int cy = right->boundingRect.height() * 0.5;
		left->Move(0, cy);
		shape->Move(left->boundingRect.width() + settings->GetValue("InterSymbolSpace", level) / 2, cy);
		right->Move(shape->boundingRect.right() + settings->GetValue("InterSymbolSpace", level) / 2, 0);

		UpdateBoundingRect();
		
		baseline = left->baseline + cy;
	}
}

/**
 * Makes a deep copy of this object.
 * @return A copy of this object.
 */
FormulaNode* PowerFormulaNode::Clone()
{
	return new PowerFormulaNode(parent, wnd);
}

/**
 * Parses the child nodes and adds the function name to the expression.
 * @param [in,out] expr The expression.
 */
void PowerFormulaNode::Parse(ParserString& expr)
{
	expr.Add("pow(", this);
	(*this)[0]->Parse(expr);
	expr.Add(",", this);
	(*this)[2]->Parse(expr);
	expr.Add(")", this);
}
