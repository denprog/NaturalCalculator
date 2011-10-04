#include "SquareRootFormulaNode.h"
#include "../Main/FormulaWnd.h"

SquareRootFormulaNode::SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();

#ifdef _DEBUG
	name = "SquareRootFormulaNode";
#endif
}

SquareRootFormulaNode::~SquareRootFormulaNode()
{
}

void SquareRootFormulaNode::Remake()
{
	childNodes->Remake();
	
	shape->ClearShapes();
	
	if (childNodes->Count() > 0)
	{
		FormulaNode* radicand = (*this)[1];
		int cx = radicand->boundingRect.height() * 5 / 11;
		int offset = radicand->boundingRect.height() / 5;
		int cy = radicand->boundingRect.height() + offset * 2;
		
		QVector<QPointF> p;
		p.push_back(QPointF(cx, cy * 0.01));
		p.push_back(QPointF(cx * 0.722, cy));
		p.push_back(QPointF(cx * 0.281, cy * 0.495));
		p.push_back(QPointF(cx * 0.061, cy * 0.544));
		p.push_back(QPointF(cx * 0.075, cy * 0.510));
		p.push_back(QPointF(cx * 0.343, cy * 0.429));
		p.push_back(QPointF(cx * 0.703, cy * 0.869));
		p.push_back(QPointF(cx * 0.934, 0));
		p.push_back(QPointF((cx * 1.32 + radicand->boundingRect.width()), 0));
		p.push_back(QPointF((cx * 1.32 + radicand->boundingRect.width()), cy * 0.01));
		p.push_back(QPointF(cx * 1.32, cy * 0.01));
		
		shape->AddPolygon(p, QColor("black"));
		
		radicand->Move(cx * 1.3, offset);

		baseline = radicand->baseline + offset;
		shape->UpdateBoundingRect();
		boundingRect = shape->boundingRect;
	}
}

void SquareRootFormulaNode::Parse(ParserString& expr)
{
	expr.Add("sqrt(", this);
	(*this)[1]->Parse(expr);
	expr.Add(")", this);
}

void SquareRootFormulaNode::UpdateBoundingRect()
{
	boundingRect = shape->boundingRect;
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

FormulaNode* SquareRootFormulaNode::Clone()
{
	return new SquareRootFormulaNode(parent, wnd);
}

void SquareRootFormulaNode::RenderCaret(const int pos, const int anchor)
{
	if (pos == 0)
	{
		QRectF r = GetDocumentPosBounds(pos);
		int cx = (*this)[1]->boundingRect.height() * 5 / 11;
		
		QGraphicsItemGroup* g = wnd->caret->caretShape;
		
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
