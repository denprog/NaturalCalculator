#include "DivisionFormulaNode.h"
#include "../FormulaNodes/ShapeFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Default constructor.
 */
DivisionFormulaNode::DivisionFormulaNode()
{
	type = DIVISION_NODE;
}

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
DivisionFormulaNode::DivisionFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	type = DIVISION_NODE;
	
	dividend = new GroupFormulaNode(parent, wnd);
	AddChild(dividend);
	shape = AddShapeNode();
	divisor = new GroupFormulaNode(parent, wnd);
	AddChild(divisor);

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
	FormulaNode::AddChild(node);
}

/**
 * Inserts a child node concerning the shape node.
 * @param [in,out] node The node.
 * @param pos The position.
 */
void DivisionFormulaNode::InsertChild(FormulaNode* node, int pos)
{
	FormulaNode::InsertChild(node, pos);
}

void DivisionFormulaNode::RemoveChildNodes()
{
	childNodes->Clear();
	dividend = NULL;
	divisor = NULL;
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
		qreal w = max(dividend->boundingRect.width() + 2, divisor->boundingRect.width() + 2);
		
		//the shape
		if (w < 200)
			shape->AddLine(0, 0, w, 0, QColor("black"));
		else
			shape->AddFillRect(0, 0, w, w / 200 > 2 ? 2 : w / 200, QColor("black"));
		
		UpdateBoundingRect();
		
		qreal s = shape->boundingRect.height() + 1;
		
		//place the elements
		dividend->Move((w - dividend->boundingRect.width()) / 2, 0);
		shape->Move(0, dividend->boundingRect.height() + s);
		divisor->Move((w - divisor->boundingRect.width()) / 2, dividend->boundingRect.height() + shape->boundingRect.height() + s * 2);

		UpdateBoundingRect();
		
		baseline = dividend->boundingRect.height() + 1;
	}
}

/**
 * Updates the bounding rectangle.
 */
void DivisionFormulaNode::UpdateBoundingRect()
{
	FormulaNode::UpdateBoundingRect();
	if (shape->boundingRect.height() == 0)
		shape->boundingRect.setHeight(1);
}

/**
 * One cannot insert a node into this node's child nodes, only into the expression.
 * @param pos The position.
 * @return false.
 */
bool DivisionFormulaNode::CanInsert()
{
	return false;
}

/**
 * Parses the child nodes and makes output expression.
 * @param [in,out] expr The expression.
 */
void DivisionFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("("), this);
	(*this)[0]->Parse(expr);
	expr.Add(std::string(")/("), this);
	(*this)[2]->Parse(expr);
	expr.Add(std::string(")"), this);
}

#ifdef TEST
std::string DivisionFormulaNode::ParseStructure()
{
	return "(" + dividend->ParseStructure() + "/" + divisor->ParseStructure() + ")";
}
#endif

bool DivisionFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '/')
	{
		DivisionFormulaNode* d = new DivisionFormulaNode(parent, parent->wnd);
		if (parent->ChildrenCount() > 0)
		{
			if (parent->type == GROUP_NODE)
				d->dividend->MoveChild((*parent->childNodes)[parent->ChildrenCount() - 1], 0);
			else
				d->dividend->MoveChild((*parent)[0], 0);
		}
		else
			d->dividend->AddChild(new EmptyFormulaNode(d->dividend));

		++begin;
		if (!FormulaNode::FromString(begin, end, d->divisor))
			d->divisor->AddChild(new EmptyFormulaNode(d->divisor));

		parent->AddChild(d);
		return true;
	}
	
	return false;
}

std::string DivisionFormulaNode::ToString()
{
	return dividend->ToString() + "/" + divisor->ToString();
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
SharedCaretState DivisionFormulaNode::GetNextPosition(SharedCaretState relativeState)
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
SharedCaretState DivisionFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
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
		QGraphicsItemGroup* g = wnd->caret->caretShape;
		
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

/**
 * Executes the remove item operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool DivisionFormulaNode::DoRemoveItem(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	
	if (pos == 1)
	{
		//removing the symbol
		bool right = any_cast<bool>(command->nodeEvent["right"]);
		if (!right)
			return false;

		command->SaveNodeState(parent);
		
		int j = parent->GetFirstLevelChildPos(this);
		int i = 0;
		while (dividend->childNodes->Count() > 0)
			parent->MoveChild((*dividend)[0], j + i++);
		int k = j + i;
		while (divisor->childNodes->Count() > 0)
			parent->MoveChild((*divisor)[0], j + i++);
		
		c->SetToNode(parent, k);
		command->afterCaretState = c;
		
		parent->RemoveChild(j + i);
		return true;
	}

	return FormulaNode::DoRemoveItem(command);
}
