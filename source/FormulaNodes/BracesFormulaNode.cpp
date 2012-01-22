#include "BracesFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The parent window.
 */
BracesFormulaNode::BracesFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, bool left, bool right) : CompoundFormulaNode(_parent, wnd)
{
	if (!left && !right)
		assert(false);
	
	leftShape = left ? AddShapeNode() : NULL;
	rightShape = right ? AddShapeNode() : NULL;
	InsertChild(new GroupFormulaNode(parent, wnd), left ? 1 : 0);

#ifdef _DEBUG
	name = "BracesFormulaNode";
#endif
}

/**
 * Destructor.
 */
BracesFormulaNode::~BracesFormulaNode()
{
}

/**
 * Remakes this object.
 */
void BracesFormulaNode::Remake()
{
	childNodes->Remake();
	
	if (leftShape)
		leftShape->ClearShapes();
	if (rightShape)
		rightShape->ClearShapes();
	
	if (childNodes->Count() > 1)
	{
		FormulaNode* expr = GetExpression(0);
		int cx = 1 + expr->GetBoundingRect().height() / 5;
		int cy = 1.2 * expr->GetBoundingRect().height();
		
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
		
		expr->Move(leftShape ? cx : 0, expr->GetBoundingRect().height() * 0.1);
		
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
			rightShape->Move(cx + expr->GetBoundingRect().width(), 0);
		}
		
		baseline = expr->GetBaseline() + expr->GetBoundingRect().height() * 0.1;
		UpdateBoundingRect();
	}
}

/**
 * Updates the bounding rectangle.
 */
void BracesFormulaNode::UpdateBoundingRect()
{
	//boundingRect = leftShape->GetBoundingRect();
	//boundingRect.moveTo(item->pos().x(), item->pos().y());
	CompoundFormulaNode::UpdateBoundingRect();
}

FormulaNode* BracesFormulaNode::GetExpression(int pos) const
{
	assert(pos == 0);
	
	return (*this)[IsShapeVisible(0) ? 1 : 0];
}

void BracesFormulaNode::ShowShape(int pos, bool show)
{
	assert(pos <= 1);
	
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
 * Parses the child nodes.
 * @param [in,out] expr The result expression.
 */
void BracesFormulaNode::Parse(ParserString& expr)
{
	if (leftShape)
		expr.Add("(", this);
	(*this)[1]->Parse(expr);
	if (rightShape)
		expr.Add(")", this);
}
	
/**
 * Renders the caret.
 * @param pos	The position.
 * @param anchor The anchor.
 */
//void BracesFormulaNode::RenderCaret(const int pos, const int anchor)
//{
//	if (pos == 0)
//	{
//		//draw the caret on the shape
//		QRectF r = GetDocumentPosBounds(pos);
//		int cx = (*this)[1]->GetBoundingRect().height() * 5 / 11;
//		
//		QGraphicsItemGroup* g = wnd->GetCaret()->caretShape;
//		
//		qDeleteAll(g->childItems());
//		g->childItems().clear();
//		
//		QGraphicsLineItem* i = new QGraphicsLineItem(r.left(), r.top(), r.left(), r.bottom(), g);
//		i->setPen(QPen("red"));
//		g->addToGroup(i);
//		i = new QGraphicsLineItem(r.left(), r.bottom(), r.left() + cx, r.bottom(), g);
//		i->setPen(QPen("red"));
//		g->addToGroup(i);
//	}
//	else
//		CompoundFormulaNode::RenderCaret(pos, anchor);
//}
	
/**
 * Executes the remove item operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool BracesFormulaNode::DoRemoveItem(NodeEvent& nodeEvent)
{
	return false;
}

//void BracesFormulaNode::SetLeftShape(bool setShape)
//{
//	assert(GetLeftShape() != setShape);
//	
//	if (setShape)
//	{
//		leftShape = InsertShapeNode(0);
//	}
//	else
//	{
//		RemoveChild(0);
//		leftShape = NULL;
//	}
//}
//
//void BracesFormulaNode::SetRightShape(bool setShape)
//{
//	assert(GetRightShape() != setShape);
//	
//	if (setShape)
//	{
//		rightShape = InsertShapeNode(GetLeftShape() ? 2 : 1);
//	}
//	else
//	{
//		RemoveChild(GetLeftShape() ? 2 : 1);
//		rightShape = NULL;
//	}
//}
