#include "PowerFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"

PowerFormulaNode::PowerFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();

#ifdef _DEBUG
	name = "PowerFormulaNode";
#endif
}

PowerFormulaNode::~PowerFormulaNode()
{
}

void PowerFormulaNode::Remake()
{
	if (childNodes->Count() > 1)
	{
		FormulaNode* left = (*this)[0];
		FormulaNode* right = (*this)[2];

		right->SetLevel(GetLesserLevel());

		childNodes->Remake();
		shape->ClearShapes();

		shape->AddFillRect(0, 0, left->boundingRect.height() / 10, left->boundingRect.height(), QColor("white"));
		shape->UpdateBoundingRect();
		
		int cy = right->boundingRect.height() - left->boundingRect.height() / 2;
		left->Move(0, cy);
		shape->Move(left->boundingRect.width() + settings->value("InterSymbolSpace").toInt(), cy);
		right->Move(shape->boundingRect.right()+ settings->value("InterSymbolSpace").toInt(), 0);

		UpdateBoundingRect();
		
		baseline = left->baseline + cy;
	}
}

FormulaNode* PowerFormulaNode::Clone()
{
	return new PowerFormulaNode(parent, wnd);
}

void PowerFormulaNode::Parse(ParserString& expr)
{
	expr.Add("pow(", this);
	(*this)[0]->Parse(expr);
	expr.Add(",", this);
	(*this)[2]->Parse(expr);
	expr.Add(")", this);
}
