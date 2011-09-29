#include "DivisionFormulaNode.h"
#include "../FormulaNodes/ShapeFormulaNode.h"
#include "../Main/FormulaWnd.h"

DivisionFormulaNode::DivisionFormulaNode()
{
}

DivisionFormulaNode::DivisionFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();

#ifdef _DEBUG
	name = "DivisionFormulaNode";
#endif
}

DivisionFormulaNode::~DivisionFormulaNode()
{
}
	
void DivisionFormulaNode::AddChild(FormulaNode* node)
{
	if (childNodes->Count() == 1)
		InsertChild(node, 0);
	else
		CompoundFormulaNode::AddChild(node);
}

void DivisionFormulaNode::InsertChild(FormulaNode* node, int pos)
{
	switch (pos)
	{
	case 0:
		CompoundFormulaNode::InsertChild(node, 0);
		break;
	case 1:
		if (childNodes->Count() == 2)
			CompoundFormulaNode::InsertChild(node, 2);
		break;
	}
}

void DivisionFormulaNode::Remake()
{
	childNodes->Remake();
	
	shape->ClearShapes();
	
	if (childNodes->Count() == 3)
	{
		FormulaNode* dividend = (*this)[0];
		FormulaNode* divisor = (*this)[2];
		qreal w = max(dividend->boundingRect.width(), divisor->boundingRect.width());
		
		shape->AddLine(0, 0, w, 0, QColor("black"));
		
		dividend->Move((w - dividend->boundingRect.width()) / 2, 0);
		shape->Move(0, dividend->boundingRect.height());
		divisor->Move((w - divisor->boundingRect.width()) / 2, dividend->boundingRect.height() + shape->boundingRect.height());
		
		UpdateBoundingRect();
		
		baseline = dividend->boundingRect.height();
	}
}

void DivisionFormulaNode::UpdateBoundingRect()
{
	FormulaNode::UpdateBoundingRect();
	shape->boundingRect.setHeight(1);
}

void DivisionFormulaNode::Parse(ParserExpression& expr)
{
	expr.Add("(", this);
	(*this)[0]->Parse(expr);
	expr.Add(")/(", this);
	(*this)[2]->Parse(expr);
	expr.Add(")", this);
}

FormulaNode* DivisionFormulaNode::Clone()
{
	return new DivisionFormulaNode(parent, wnd);
}

SharedCaretState DivisionFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	if (!relativeState)
		return CompoundFormulaNode::GetNextPosition(relativeState);
	
	SharedCaretState res = CompoundFormulaNode::GetNextPosition(relativeState);
	//FormulaNode* n = res->GetNode();
	//FormulaNode* dividend = (*this)[0];
	//if (!res->CheckOnNode(this) && n != dividend && dividend->IsChild(n))
	//	return parent->GetNextPosition(relativeState);
	
	return res;
}

SharedCaretState DivisionFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	if (!relativeState)
		return CompoundFormulaNode::GetPreviousPosition(relativeState);
	
	//FormulaNode* divisor = (*this)[2];
	SharedCaretState res = CompoundFormulaNode::GetPreviousPosition(relativeState);
	//if (!res.Empty())
	//{
	//	FormulaNode* n = res.GetNode();
	//	if (!res.CheckOnNode(this) && n != divisor && !divisor->IsChild(n))
	//		return parent->GetPreviousPosition(relativeState);
	//}
	
	return res;
}

void DivisionFormulaNode::RenderCaret(const int pos, const int anchor)
{
	if (pos == 1)
	{
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
