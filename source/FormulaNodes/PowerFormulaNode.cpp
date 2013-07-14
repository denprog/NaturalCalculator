#include "PowerFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "../Main/Settings.h"

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
PowerFormulaNode::PowerFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	type = POWER_NODE;
	base = new GroupFormulaNode(this, wnd);
	AddChild(base);
	shape = AddShapeNode();
	exponent = new GroupFormulaNode(this, wnd);
	AddChild(exponent);
}

PowerFormulaNode::PowerFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FormulaNode* _base, FormulaNode* _exponent) : 
	CompoundFormulaNode(_parent, wnd)
{
	base->AddChild(_base);
	exponent->AddChild(_exponent);
	type = POWER_NODE;
	FormulaNode::AddChild(base);
	shape = AddShapeNode();
	FormulaNode::AddChild(exponent);
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
		exponent->SetLevel(GetLesserLevel());

		childNodes->Remake();
		shape->ClearShapes();

		//the shape
		shape->AddFillRect(0, 0, base->boundingRect.height() / 10, exponent->boundingRect.height() + base->boundingRect.height() / 2, QColor("white"));
		shape->UpdateBoundingRect();
		
		int cy = exponent->boundingRect.height() - base->boundingRect.height() / 2;
		base->Move(0, cy);
		shape->Move(base->boundingRect.width() + settings->GetValue("InterSymbolSpace", level) / 2, 0);
		exponent->Move(shape->boundingRect.right() + settings->GetValue("InterSymbolSpace", level) / 2, 0);

		UpdateBoundingRect();
		
		baseline = base->baseline + cy;
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
	base->Parse(expr);
	expr.Add(std::string(","), this);
	exponent->Parse(expr);
	expr.Add(std::string(")"), this);
}

#ifdef TEST
void PowerFormulaNode::ParseStructure(QString& res)
{
	res += "pow(";
	(*this)[0]->ParseStructure(res);
	res += ",";
	(*this)[2]->ParseStructure(res);
	res += ")";
}
#endif

bool PowerFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	std::string res;
	std::string::iterator i = begin;
	while (i != end)
	{
		res += *i;
		++i;
		if (res.size() == 3)
			break;
	}
	
	if (res == "pow")
	{
		PowerFormulaNode* t = new PowerFormulaNode(parent, parent->wnd);
		
		if (!GroupFormulaNode::FromString(i, end, parent))
		{
			delete t;
			return false;
		}
		
		FormulaNode* g = (*parent->childNodes)[parent->ChildrenCount() - 1];
		FormulaNode* c = t->base;
		for (int j = 0, k = 0; j < g->ChildrenCount();)
		{
			FormulaNode* n = (*g)[j];
			if (n->type == COMMA_NODE)
			{
				++j;
				k = 0;
				c = t->exponent;
				continue;
			}
			c->MoveChild(n, k++);
		}
		
		parent->RemoveChild(parent->ChildrenCount() - 1);
		
		if (t->base->ChildrenCount() == 0)
			t->base->AddChild(new EmptyFormulaNode(t->base));
		if (t->exponent->ChildrenCount() == 0)
			t->exponent->AddChild(new EmptyFormulaNode(t->exponent));
			
		parent->AddChild(t);
		
		begin = i;
		return true;
	}
	
	return false;
}

std::string PowerFormulaNode::ToString()
{
	return "pow(" + base->ToString() + "," + exponent->ToString() + ")";
}
