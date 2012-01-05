#include "DivisionFormulaNode.h"
#include "../FormulaNodes/ShapeFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Default constructor.
 */
DivisionFormulaNode::DivisionFormulaNode()
{
}

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
DivisionFormulaNode::DivisionFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();

#ifdef _DEBUG
	name = "DivisionFormulaNode";
#endif
}

/**
 * Destructor.
 */
DivisionFormulaNode::~DivisionFormulaNode()
{
}
	
/**
 * Adds a child node.
 * @param [in,out] node If non-null, the node.
 */
void DivisionFormulaNode::AddChild(FormulaNode* node)
{
	if (childNodes->Count() == 1)
		InsertChild(node, 0);
	else
		CompoundFormulaNode::AddChild(node);
}

/**
 * Inserts a child node concerning the shape node.
 * @param [in,out] node The node.
 * @param pos The position.
 */
void DivisionFormulaNode::InsertChild(FormulaNode* node, int pos)
{
	switch (pos)
	{
	case 0:
		CompoundFormulaNode::InsertChild(node, 0);
		break;
	case 1: //the shape node
	case 2:
		if (childNodes->Count() == 2)
			CompoundFormulaNode::InsertChild(node, 2);
		break;
	}
}

/**
 * Remakes this node.
 */
void DivisionFormulaNode::Remake()
{
	childNodes->Remake();
	
	shape->ClearShapes();
	
	if (childNodes->Count() == 3)
	{
		FormulaNode* dividend = (*this)[0];
		FormulaNode* divisor = (*this)[2];
		qreal w = max(dividend->GetBoundingRect().width(), divisor->GetBoundingRect().width());
		
		//the shape
		shape->AddLine(0, 0, w, 0, QColor("black"));
		
		//place the elements
		dividend->Move((w - dividend->GetBoundingRect().width()) / 2, 0);
		shape->Move(0, dividend->GetBoundingRect().height());
		divisor->Move((w - divisor->GetBoundingRect().width()) / 2, dividend->GetBoundingRect().height() + shape->GetBoundingRect().height());

		UpdateBoundingRect();
		
		baseline = dividend->GetBoundingRect().height() + 1;
	}
}

/**
 * Updates the bounding rectangle.
 */
void DivisionFormulaNode::UpdateBoundingRect()
{
	FormulaNode::UpdateBoundingRect();
	shape->GetBoundingRect().setHeight(1);
}

/**
 * Parses the child nodes and makes output expression.
 * @param [in,out] expr The expression.
 */
void DivisionFormulaNode::Parse(ParserString& expr)
{
	expr.Add("(", this);
	(*this)[0]->Parse(expr);
	expr.Add(")/(", this);
	(*this)[2]->Parse(expr);
	expr.Add(")", this);
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* DivisionFormulaNode::Clone(FormulaNode* p)
{
	DivisionFormulaNode* res = new DivisionFormulaNode(p, wnd);
	res->InsertChild((*this)[0]->Clone(res), 0);
	res->InsertChild((*this)[2]->Clone(res), 2);
	
	return res;
}

/**
 * Gets the next caret position of the node.
 * @param [in,out] relativeState Relative caret state.
 * @return The next position.
 */
SharedCaretState DivisionFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	if (!relativeState)
		return CompoundFormulaNode::GetNextPosition(relativeState);
	
	return CompoundFormulaNode::GetNextPosition(relativeState);
}

/**
 * Gets the previous caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The previous position.
 */
SharedCaretState DivisionFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	if (!relativeState)
		return CompoundFormulaNode::GetPreviousPosition(relativeState);
	
	return CompoundFormulaNode::GetPreviousPosition(relativeState);
}

/**
 * Renders the caret inside this node.
 * @param pos	The position.
 * @param anchor The anchor.
 */
void DivisionFormulaNode::RenderCaret(const int pos, const int anchor)
{
	if (pos == 1)
	{
		//render the shape's caret
		QRectF r = GetDocumentPosBounds(pos);
		QGraphicsItemGroup* g = wnd->GetCaret()->caretShape;
		
		qDeleteAll(g->childItems());
		g->childItems().clear();
		
		QGraphicsLineItem* i = new QGraphicsLineItem(r.left(), r.top() - 2, r.left(), r.bottom() + 2, g);
		i->setPen(QPen("red"));
		g->addToGroup(i);
		i = new QGraphicsLineItem(r.left(), r.bottom() + 2, r.right(), r.bottom() + 2, g);
		i->setPen(QPen("red"));
		g->addToGroup(i);
	}
	else
		CompoundFormulaNode::RenderCaret(pos, anchor);
}
