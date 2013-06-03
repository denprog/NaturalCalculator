#include "ShapeFormulaNode.h"

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The formula window.
 */
ShapeFormulaNode::ShapeFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd)
{
	type = SHAPE_NODE;
}

/**
 * Destructor.
 */
ShapeFormulaNode::~ShapeFormulaNode()
{
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* ShapeFormulaNode::Clone(FormulaNode* p)
{
	return new ShapeFormulaNode(p, wnd);
}

#ifdef TEST
void ShapeFormulaNode::ParseStructure(QString& res)
{
	res += "s";
}
#endif

/**
 * Adds a line.
 * @param x1 The first x value.
 * @param y1 The first y value.
 * @param x2 The second x value.
 * @param y2 The second y value.
 * @param [in] color The color.
 */
void ShapeFormulaNode::AddLine(qreal x1, qreal y1, qreal x2, qreal y2, QColor color)
{
	QGraphicsLineItem* i = new QGraphicsLineItem(x1, y1, x2, y2, item);
	i->setPen(QPen(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

/**
 * Adds a rectangle.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width.
 * @param height The height.
 * @param [in] color The color.
 */
void ShapeFormulaNode::AddRect(qreal x, qreal y, qreal width, qreal height, QColor color)
{
	QGraphicsRectItem* i = new QGraphicsRectItem(x, y, width, height, item);
	i->setPen(QPen(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

/**
 * Adds a fill rectangle.
 * @param x	The x coordinate.
 * @param y	The y coordinate.
 * @param width	The width.
 * @param height The height.
 * @param [in] color The color.
 */
void ShapeFormulaNode::AddFillRect(qreal x, qreal y, qreal width, qreal height, QColor color, qreal opacity)
{
	QGraphicsRectItem* i = new QGraphicsRectItem(x, y, width, height, item);
	i->setBrush(QBrush(color));
	i->setPen(QPen(color));
	i->setOpacity(opacity);
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

/**
 * Adds a polygon.
 * @param [in] points The points.
 * @param [in] color The color.
 */
void ShapeFormulaNode::AddPolygon(QVector<QPointF>& points, QColor color, bool smooth)
{
	QGraphicsPolygonItem* i = smooth ? new SmoothGraphicsPolygonItem(points, item) : new QGraphicsPolygonItem(points, item);
	i->setBrush(QBrush(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
	item->update();
}

/**
 * Adds a fill circle.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius.
 * @param [in,out] color The color.
 */
void ShapeFormulaNode::AddFillCircle(qreal x, qreal y, qreal radius, QColor color, bool smooth)
{
	QGraphicsEllipseItem* i = smooth ? new SmoothGraphicsEllipseItem(x, y, radius, radius, item) : new QGraphicsEllipseItem(x, y, radius, radius, item);
	i->setBrush(QBrush(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

/**
 * Adds a path to the item.
 * @param [in] path The painter's path.
 * @param [in] color The color.
 */
void ShapeFormulaNode::AddPath(QPainterPath path, QColor color, bool smooth)
{
	QGraphicsPathItem* i = smooth ? new SmoothGraphicsPathItem(path, item) : new QGraphicsPathItem(path, item);
	i->setBrush(QBrush(color));
	((QGraphicsItemGroup*)item)->addToGroup(i);
}

/**
 * Clears the shapes.
 */
void ShapeFormulaNode::ClearShapes()
{
	foreach (QGraphicsItem *i, item->childItems())
	{
		((QGraphicsItemGroup*)item)->removeFromGroup(i);
		delete i;
	}

	item->childItems().clear();
}

/**
 * Gets the next caret position.
 * @param [in] relativeState Relative caret state.
 * @return The next caret position.
 */
SharedCaretState ShapeFormulaNode::GetNextPosition(SharedCaretState relativeState)
{
	return SharedCaretState();
}

/**
 * Gets the previous caret position.
 * @param [in] relativeState Relative caret state.
 * @return The previous caret position.
 */
SharedCaretState ShapeFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
{
	if (relativeState && relativeState->CheckOnNode(this))
		return SharedCaretState();
	
	return SharedCaretState(new CaretState(parent, parent->GetChildPos(this)));
}
