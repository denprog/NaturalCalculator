#include "BracesFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The parent window.
 */
BracesFormulaNode::BracesFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, bool left, bool right) : CompoundFormulaNode(_parent, wnd), inside(NULL)
{
	type = BRACES_NODE;

	if (!left && !right)
		assert(false);
	
	leftShape = left ? AddShapeNode() : NULL;
	rightShape = right ? AddShapeNode() : NULL;
	inside = new GroupFormulaNode(this, wnd);
	InsertChild(inside, left ? 1 : 0);
}

/**
 * Destructor.
 */
BracesFormulaNode::~BracesFormulaNode()
{
}

void BracesFormulaNode::AddChild(FormulaNode* node)
{
	if (inside)
		inside->AddChild(node);
	else
		FormulaNode::AddChild(node);
}

void BracesFormulaNode::RemoveChildNodes()
{
	childNodes->Clear();
	leftShape = NULL;
	inside = NULL;
	rightShape = NULL;
}

/**
 * Remakes this object.
 */
void BracesFormulaNode::Remake()
{
	if (IsShapeVisible(0) && IsShapeVisible(1) && inside->ChildrenCount() == 0)
		inside->AddChild(new EmptyFormulaNode(this));
	
	childNodes->Remake();
	
	if (leftShape)
		leftShape->ClearShapes();
	if (rightShape)
		rightShape->ClearShapes();
	
	if (childNodes->Count() > 1)
	{
		FormulaNode* expr = GetExpression(0);
		int cx = 1 + expr->boundingRect.height() / 5;
		int cy = 1.2 * expr->boundingRect.height();
		
		if (leftShape)
		{
			QPainterPath p(QPointF(cx, cy));
			p.cubicTo(cx * 0.59, cy * 0.941, cx * 0.132, cy * 0.66, cx * 0.132, cy * 0.5);
			p.cubicTo(cx * 0.132, cy * 0.333, cx * 0.613, cy * 0.059, cx, 0);
			p.lineTo(cx, cy * 0.023);
			p.cubicTo(cx, cy * 0.023, cx * 0.807, cy * 0.059, cx * 0.558, cy * 0.19);
			p.cubicTo(cx * 0.436, cy * 0.38, cx * 0.436, cy * 0.486, cx * 0.436, cy * 0.606);
			p.cubicTo(cx * 0.549, cy * 0.796, cx * 0.781, cy * 0.926, cx, cy * 0.977);
			p.lineTo(cx, cy);
			
			leftShape->AddPath(p, QColor("black"));
			leftShape->UpdateBoundingRect();
		}
		
		expr->Move(leftShape ? cx : 0, expr->boundingRect.height() * 0.1);
		
		if (rightShape)
		{
			QPainterPath p(QPointF(0, cy));
			p.cubicTo(cx * 0.41, cy * 0.941, cx * 0.868, cy * 0.66, cx * 0.868, cy * 0.5);
			p.cubicTo(cx * 0.868, cy * 0.333, cx * 0.387, cy * 0.059, 0, 0);
			p.lineTo(0, cy * 0.023);
			p.cubicTo(0, cy * 0.023, cx * 0.193, cy * 0.059, cx * 0.442, cy * 0.19);
			p.cubicTo(cx * 0.564, cy * 0.38, cx * 0.564, cy * 0.486, cx * 0.564, cy * 0.606);
			p.cubicTo(cx * 0.451, cy * 0.726, cx * 0.219, cy * 0.926, 0, cy * 0.977);
			p.lineTo(0, cy);

			rightShape->AddPath(p, QColor("black"));
			rightShape->UpdateBoundingRect();
			rightShape->Move((leftShape ? cx : 0) + expr->boundingRect.width(), 0);
		}
		
		baseline = expr->baseline + expr->boundingRect.height() * 0.1;
		UpdateBoundingRect();
	}
}

/**
 * One cannot insert a node into this node's child nodes, only into the expression.
 * @param pos The position.
 * @return false.
 */
bool BracesFormulaNode::CanInsert(int pos)
{
	return false;
}

/**
 * Returns an expression of the node.
 * @param pos The position.
 * @return null if it fails, else the expression.
 */
FormulaNode* BracesFormulaNode::GetExpression(int pos) const
{
	assert(pos == 0);
	
	return (*this)[IsShapeVisible(0) ? 1 : 0];
}

/**
 * Shows the shape.
 * @param pos The position.
 * @param show true to show, false to hide.
 */
void BracesFormulaNode::ShowShape(int pos, bool show)
{
	assert(pos <= 2);
	
	if (show)
	{
		if (pos == 0)
			leftShape = InsertShapeNode(0);
		else
			rightShape = InsertShapeNode(IsShapeVisible(0) ? 2 : 1);
	}
	else
	{
		RemoveChild(pos == 0 ? 0 : (IsShapeVisible(0) ? 2 : 1));
		pos == 0 ? leftShape = NULL : rightShape = NULL;
	}
}

/**
 * Query if a shape is visible at the pos.
 * @param pos The position.
 * @return true if shape visible, false if not.
 */
bool BracesFormulaNode::IsShapeVisible(int pos) const
{
	assert(pos <= 1);
	
	if (pos == 0)
		return leftShape != NULL;
	return rightShape != NULL;
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return null if it fails, else a copy of this object.
 */
FormulaNode* BracesFormulaNode::Clone(FormulaNode* p)
{
	BracesFormulaNode* res = new BracesFormulaNode(p, wnd, IsShapeVisible(0), IsShapeVisible(1));
	res->InsertChild(GetExpression(0)->Clone(res), IsShapeVisible(0) ? 1 : 0);
	
	return res;
}

/**
 * Returns the last position of the caret.
 * @return The last position.
 */
SharedCaretState BracesFormulaNode::GetLastPosition()
{
	if (rightShape)
		return SharedCaretState(new CaretState(rightShape));
	return GetExpression(0)->GetLastPosition();
}

/**
 * Parses the child nodes.
 * @param [in,out] expr The result expression.
 */
void BracesFormulaNode::Parse(ParserString& expr)
{
	if (leftShape)
		expr.Add(std::string("("), this);
	(*this)[1]->Parse(expr);
	if (rightShape)
		expr.Add(std::string(")"), this);
}

#ifdef TEST
std::string BracesFormulaNode::ParseStructure()
{
	std::string res;
	if (leftShape)
		res += "[";
	res += inside->ParseStructure();
	if (rightShape)
		res += "]";
	return res;
}
#endif

bool BracesFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '[')
	{
		BracesFormulaNode* b = new BracesFormulaNode(parent, parent->wnd, true, false);
		parent->AddChild(b);
		++begin;
		
		while (begin != end)
		{
			if (!FormulaNode::FromString(begin, end, b->inside))
				break;
		}
		
		return true;
	}
	else if (*begin == ']')
	{
		if (parent->parent != NULL && parent->parent->type == BRACES_NODE)
		{
			parent->parent->ShowShape(1, true);
			++begin;
			return false;
		}
		else
		{
			BracesFormulaNode* b = new BracesFormulaNode(parent, parent->wnd, false, true);
			int i = 0;
			while (parent->ChildrenCount() > 0)
				b->inside->MoveChild((*parent)[0], i++);
			parent->AddChild(b);
		}

		++begin;
		return true;
	}

	return false;
}

std::string BracesFormulaNode::ToString()
{
	std::string res;
	if (leftShape)
		res += "[";
	res += inside->ToString();
	if (rightShape)
		res += "]";
	return res;
}

/**
 * Executes the remove item operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool BracesFormulaNode::DoRemoveItem(Command* command)
{
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	
	if (pos == 0 || pos == 2 || (pos == 1 && leftShape == NULL))
	{
		bool right = any_cast<bool>(command->nodeEvent["right"]);
		if (!right)
			return false;

		command->SaveNodeState(parent);
		
		int j = parent->GetFirstLevelChildPos(this);
		if ((pos == 0 && rightShape == NULL) || (pos == 1 && leftShape == NULL))
		{
			//removing the node itself
			int i = 0;
			while (inside->childNodes->Count() > 0)
				parent->MoveChild((*inside)[0], j + i++);

			c->SetToNode(parent, pos == 0 ? j : j + i);
			parent->RemoveChild(j + i);
		}
		else
		{
			//removing a symbol
			ShowShape(pos, 0);
			//move nodes inside this one
			if (pos == 0)
			{
				int k = inside->ChildrenCount();
				inside->MoveChildren(parent, 0, j, 0);
				if (k == inside->ChildrenCount())
					c = inside->GetFirstPosition();
				else
					c->SetToNode(inside, j);
			}
			else
			{
				int k = inside->ChildrenCount();
				inside->MoveChildren(parent, j + 1, parent->ChildrenCount(), inside->ChildrenCount());
				if (k == inside->ChildrenCount())
					c = inside->GetLastPosition();
				else
					c->SetToNode(inside, k + 1);
			}
		}

		command->afterCaretState = c;
		
		return true;
	}

	return FormulaNode::DoRemoveItem(command);
}

SharedCaretState BracesFormulaNode::GetNextPosition(SharedCaretState relativeState)
{
	if (!relativeState)
		return SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
	else
	{
		if (relativeState->CheckInNode(this) && relativeState->GetPos() == 1)
		{
			if (leftShape && !rightShape)
			{
				SharedCaretState c = CompoundFormulaNode::GetNextPosition(relativeState);
				return parent->GetNextPosition(c);
			}
		}
	}
	
	return CompoundFormulaNode::GetNextPosition(relativeState);
}

SharedCaretState BracesFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
{
	if (!relativeState)
		return CompoundFormulaNode::GetPreviousPosition(relativeState);
	else
	{
		if (relativeState->CheckInNode(this) && relativeState->GetPos() == 0)
		{
			if (!leftShape && rightShape)
			{
				SharedCaretState c = CompoundFormulaNode::GetPreviousPosition(relativeState);
				return parent->GetPreviousPosition(c);
			}
		}
	}
	
	return CompoundFormulaNode::GetPreviousPosition(relativeState);
}
