#include "ShapeFormulaNode.h"

ShapeFormulaNode::ShapeFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd)
{
#ifdef _DEBUG
	name = "ShapeFormulaNode";
#endif
}

ShapeFormulaNode::~ShapeFormulaNode()
{
}

FormulaNode* ShapeFormulaNode::Clone()
{
	return new ShapeFormulaNode(parent, wnd);
}

void ShapeFormulaNode::AddLine(qreal x1, qreal y1, qreal x2, qreal y2, QColor& color)
{
	QGraphicsLineItem* i = new QGraphicsLineItem(x1, y1, x2, y2, item);
	i->setPen(QPen(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

void ShapeFormulaNode::AddRect(qreal x, qreal y, qreal width, qreal height, QColor& color)
{
	QGraphicsRectItem* i = new QGraphicsRectItem(x, y, width, height, item);
	i->setPen(QPen(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

void ShapeFormulaNode::AddFillRect(qreal x, qreal y, qreal width, qreal height, QColor& color)
{
	QGraphicsRectItem* i = new QGraphicsRectItem(x, y, width, height, item);
	i->setBrush(QBrush(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

void ShapeFormulaNode::AddPolygon(QVector<QPointF>& points, QColor& color)
{
	QGraphicsPolygonItem* i = new QGraphicsPolygonItem(points, item);
	i->setBrush(QBrush(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
	item->update();
}

void ShapeFormulaNode::AddFillCircle(qreal x, qreal y, qreal radius, QColor& color)
{
	QGraphicsEllipseItem* i = new QGraphicsEllipseItem(x, y, radius, radius, item);
	i->setBrush(QBrush(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

void ShapeFormulaNode::ClearShapes()
{
	foreach (QGraphicsItem *i, item->childItems())
	{
		((QGraphicsItemGroup*)item)->removeFromGroup(i);
		//i->setParentItem(NULL);
		delete i;
	}
	
	//qDeleteAll(item->childItems());

	item->childItems().clear();
}

SharedCaretState ShapeFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	return SharedCaretState();
}

SharedCaretState ShapeFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	if (relativeState && relativeState->CheckOnNode(this))
		return SharedCaretState();
	
	return SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
}
