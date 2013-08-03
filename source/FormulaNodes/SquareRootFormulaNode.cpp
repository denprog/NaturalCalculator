#include "SquareRootFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Constructor.
 * @param [in] _parent The parent.
 * @param [in] wnd The formula window.
 */
SquareRootFormulaNode::SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	type = SQUARE_ROOT_NODE;
	shape = AddShapeNode();
	radicand = new GroupFormulaNode(parent, wnd);
	InsertChild(radicand, 1);
}

SquareRootFormulaNode::SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FormulaNode* _radicand) : CompoundFormulaNode(_parent, wnd)
{
	type = SQUARE_ROOT_NODE;
	shape = AddShapeNode();
	radicand->AddChild(_radicand);
	InsertChild(radicand, 1);
}

/**
 * Destructor.
 */
SquareRootFormulaNode::~SquareRootFormulaNode()
{
}

void SquareRootFormulaNode::RemoveChildNodes()
{
	childNodes->Clear();
	radicand = NULL;
}

/**
 * Remakes this node.
 */
void SquareRootFormulaNode::Remake()
{
	childNodes->Remake();
	
	shape->ClearShapes();
	
	if (childNodes->Count() > 0)
	{
		qreal cx = radicand->boundingRect.height() * 5 / 11;
		qreal offset = radicand->boundingRect.height() / 5;
		qreal cy = radicand->boundingRect.height() + offset * 2;
		
		QVector<QPointF> p;
		p.push_back(QPointF(cx, cy * 0.01));
		p.push_back(QPointF(cx * 0.722, cy));
		p.push_back(QPointF(cx * 0.281, cy * 0.495));
		p.push_back(QPointF(cx * 0.061, cy * 0.544));
		p.push_back(QPointF(cx * 0.075, cy * 0.510));
		p.push_back(QPointF(cx * 0.343, cy * 0.429));
		p.push_back(QPointF(cx * 0.703, cy * 0.869));
		p.push_back(QPointF(cx * 0.934, 0));
		p.push_back(QPointF(cx * 1.3 + radicand->boundingRect.width() + 1, 0));
		p.push_back(QPointF(cx * 1.3 + radicand->boundingRect.width() + 1, cy * 0.01));
		p.push_back(QPointF(cx * 1.3, cy * 0.01));
		
		shape->AddPolygon(p, QColor("black"));
		
		radicand->Move(cx * 1.3, offset);

		baseline = radicand->baseline + offset;
		shape->UpdateBoundingRect();
		boundingRect = shape->boundingRect;
		boundingRect.setRight(boundingRect.right() + cx * 0.075);
	}
}

/**
 * One cannot insert a node into this node's child nodes, only into the expression.
 * @param pos The position.
 * @return false.
 */
bool SquareRootFormulaNode::CanInsert(int pos)
{
	return false;
}

/**
 * Returns an expression of the node.
 * @param pos The position.
 * @return null if it fails, else the expression.
 */
FormulaNode* SquareRootFormulaNode::GetExpression(int pos) const
{
	assert(pos == 0);
	return (*this)[1];
}

/**
 * The shape of the node is always visible.
 * @param pos Fake parameter.
 * @param show Fake parameter.
 */
void SquareRootFormulaNode::ShowShape(int pos, bool show)
{
}

/**
 * The shape of the node is always visible.
 * @param pos The position.
 * @return true.
 */
bool SquareRootFormulaNode::IsShapeVisible(int pos) const
{
	assert(pos == 0);
	return true;
}

/**
 * Parses child node and adds the function call.
 * @param [in,out] expr The expression.
 */
void SquareRootFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("sqrt("), this);
	GetExpression(0)->Parse(expr);
	expr.Add(std::string(")"), this);
}

#ifdef TEST
std::string SquareRootFormulaNode::ParseStructure()
{
	return "sqrt(" + radicand->ParseStructure() + ")";
}
#endif

bool SquareRootFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	std::string res;
	std::string::iterator i = begin;
	while (i != end)
	{
		res += *i;
		++i;
		if (res.size() == 4)
			break;
	}
	
	if (res == "sqrt")
	{
		SquareRootFormulaNode* t = new SquareRootFormulaNode(parent, parent->wnd);
		
		if (!GroupFormulaNode::FromString(i, end, t->radicand))
		{
			delete t;
			return false;
		}
		
		if (t->radicand->ChildrenCount() == 0)
			t->radicand->AddChild(new EmptyFormulaNode(t->radicand));
			
		parent->AddChild(t);
		
		begin = i;
		return true;
	}
	
	return false;
}

std::string SquareRootFormulaNode::ToString()
{
	return "sqrt(" + radicand->ToString() + ")";
}

/**
 * Updates the bounding rectangle.
 */
void SquareRootFormulaNode::UpdateBoundingRect()
{
	boundingRect = shape->boundingRect;
	qreal cx = GetExpression(0)->boundingRect.height() * 5 / 11;
	boundingRect.setRight(boundingRect.right() + cx * 0.075);
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* SquareRootFormulaNode::Clone(FormulaNode* p)
{
	SquareRootFormulaNode* res = new SquareRootFormulaNode(p, wnd);
	res->InsertChild(GetExpression(0)->Clone(res), 1);
	
	return res;
}

/**
 * Renders the caret inside this node.
 * @param pos	The position.
 * @param anchor The anchor.
 */
void SquareRootFormulaNode::RenderCaret(const int pos, const int anchor)
{
	if (pos == 0)
	{
		//draw the caret on the shape
		QRectF r = GetDocumentPosBounds(pos);
		int cx = GetExpression(0)->boundingRect.height() * 5 / 11;
		
		QGraphicsItemGroup* g = wnd->GetCaret()->caretShape;
		
		qDeleteAll(g->childItems());
		g->childItems().clear();
		
		QGraphicsLineItem* i = new QGraphicsLineItem(r.left(), r.top(), r.left(), r.bottom(), g);
		i->setPen(QPen("red"));
		g->addToGroup(i);
		i = new QGraphicsLineItem(r.left(), r.bottom(), r.left() + cx, r.bottom(), g);
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
bool SquareRootFormulaNode::DoRemoveItem(Command* command)
{
	command->SaveNodeState(parent);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	
	if (pos == 0)
	{
		//removing the symbol
		bool right = any_cast<bool>(command->nodeEvent["right"]);
		if (!right)
			return false;
		
		int j = parent->GetFirstLevelChildPos(this);
		int i = 0;
		while (radicand->childNodes->Count() > 0)
			parent->MoveChild((*radicand)[0], j + i++);
		c->SetToNode(parent, j);
		command->afterCaretState = c;

		parent->RemoveChild(j + i);		
		return true;
	}

	return FormulaNode::DoRemoveItem(command);
}
