#ifndef SHAPEFORMULANODE_H
#define SHAPEFORMULANODE_H

#include "GroupFormulaNode.h"
#include <QPainter>

/**
 * Shape formula node.
 */
class ShapeFormulaNode : public GroupFormulaNode
{
public:
	ShapeFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~ShapeFormulaNode();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar << boost::serialization::base_object<GroupFormulaNode>(*this);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar >> boost::serialization::base_object<GroupFormulaNode>(*this);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	virtual FormulaNode* Clone(FormulaNode* p);

#ifdef TEST
	virtual void ParseStructure(QString& res);
#endif

	void AddLine(qreal x1, qreal y1, qreal x2, qreal y2, QColor color);
	void AddRect(qreal x, qreal y, qreal width, qreal height, QColor color);
	void AddFillRect(qreal x, qreal y, qreal width, qreal height, QColor color, qreal opacity = 1.0);
	void AddPolygon(QVector<QPointF>& points, QColor color, bool smooth = true);
	void AddFillCircle(qreal x, qreal y, qreal radius, QColor color, bool smooth = true);
	void AddPath(QPainterPath path, QColor color, bool smooth = true);
	void ClearShapes();

	virtual SharedCaretState GetNextPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState relativeState = SharedCaretState());
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const ShapeFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, ShapeFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)ShapeFormulaNode(parent, parent->wnd);
		}
	}
}

/**
 * Smooth polygon item.
 */
class SmoothGraphicsPolygonItem : public QGraphicsPolygonItem
{
public:
	/**
	 * Constructor.
	 * @param polygon The polygon.
	 * @param parent (optional) [in,out] The parent item.
	 */
	SmoothGraphicsPolygonItem(const QPolygonF& polygon, QGraphicsItem* parent = 0) : QGraphicsPolygonItem(polygon, parent)
	{
	}
	
public:
	/**
	 * Paint handler that adds the anti-aliasing.
	 * @param [in,out] painter The painter.
	 * @param option The option.
	 * @param [in,out] widget The widget.
	 */
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
		QGraphicsPolygonItem::paint(painter, option, widget);
	}
};

/**
 * Smooth ellipse item.
 */
class SmoothGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
	/**
	 * Constructor.
	 * @param x	The x coordinate.
	 * @param y	The y coordinate.
	 * @param width The width.
	 * @param height The height.
	 * @param parent (optional) [in,out] The parent item.
	 */
	SmoothGraphicsEllipseItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = 0) : QGraphicsEllipseItem(x, y, width, height, parent)
	{
	}
	
public:
	/**
	 * Paint handler that adds the anti-aliasing.
	 * @param [in,out] painter The painter.
	 * @param option The option.
	 * @param [in,out] widget The widget.
	 */
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
		QGraphicsEllipseItem::paint(painter, option, widget);
	}
};

/**
 * Smooth path item.
 */
class SmoothGraphicsPathItem : public QGraphicsPathItem
{
public:
	/**
	 * Constructor.
	 * @param path The path.
	 * @param parent (optional) [in,out] The parent item.
	 */
	SmoothGraphicsPathItem(const QPainterPath& path, QGraphicsItem* parent = 0) : QGraphicsPathItem(path, parent)
	{
	}
	
public:
	/**
	 * Paint handler that adds the anti-aliasing.
	 * @param [in,out] painter The painter.
	 * @param option The option.
	 * @param [in,out] widget The widget.
	 */
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
		QGraphicsPathItem::paint(painter, option, widget);
	}
};

#endif
