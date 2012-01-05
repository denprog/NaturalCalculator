#include "SquareRootFormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Constructor.
 * @param [in] _parent The parent.
 * @param [in] wnd The formula window.
 */
SquareRootFormulaNode::SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();

#ifdef _DEBUG
	name = "SquareRootFormulaNode";
#endif
}

/**
 * Destructor.
 */
SquareRootFormulaNode::~SquareRootFormulaNode()
{
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
		FormulaNode* radicand = (*this)[1];
		int cx = radicand->GetBoundingRect().height() * 5 / 11;
		int offset = radicand->GetBoundingRect().height() / 5;
		int cy = radicand->GetBoundingRect().height() + offset * 2;
		
		QVector<QPointF> p;
		p.push_back(QPointF(cx, cy * 0.01));
		p.push_back(QPointF(cx * 0.722, cy));
		p.push_back(QPointF(cx * 0.281, cy * 0.495));
		p.push_back(QPointF(cx * 0.061, cy * 0.544));
		p.push_back(QPointF(cx * 0.075, cy * 0.510));
		p.push_back(QPointF(cx * 0.343, cy * 0.429));
		p.push_back(QPointF(cx * 0.703, cy * 0.869));
		p.push_back(QPointF(cx * 0.934, 0));
		p.push_back(QPointF((cx * 1.32 + radicand->GetBoundingRect().width()), 0));
		p.push_back(QPointF((cx * 1.32 + radicand->GetBoundingRect().width()), cy * 0.01));
		p.push_back(QPointF(cx * 1.32, cy * 0.01));
		
		shape->AddPolygon(p, QColor("black"));
		
		radicand->Move(cx * 1.3, offset);

		baseline = radicand->GetBaseline() + offset;
		shape->UpdateBoundingRect();
		boundingRect = shape->GetBoundingRect();
	}
}

/**
 * Parses child node and adds the function call.
 * @param [in,out] expr The expression.
 */
void SquareRootFormulaNode::Parse(ParserString& expr)
{
	expr.Add("sqrt(", this);
	(*this)[1]->Parse(expr);
	expr.Add(")", this);
}

/**
 * Updates the bounding rectangle.
 */
void SquareRootFormulaNode::UpdateBoundingRect()
{
	boundingRect = shape->GetBoundingRect();
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
	res->InsertChild((*this)[1]->Clone(res), 1);
	
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
		int cx = (*this)[1]->GetBoundingRect().height() * 5 / 11;
		
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
