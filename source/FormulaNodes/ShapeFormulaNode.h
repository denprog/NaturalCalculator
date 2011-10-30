#ifndef SHAPEFORMULANODE_H
#define SHAPEFORMULANODE_H

#include "GroupFormulaNode.h"

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
	virtual FormulaNode* Clone();

	void AddLine(qreal x1, qreal y1, qreal x2, qreal y2, QColor& color);
	void AddRect(qreal x, qreal y, qreal width, qreal height, QColor& color);
	void AddFillRect(qreal x, qreal y, qreal width, qreal height, QColor& color);
	void AddPolygon(QVector<QPointF>& points, QColor& color);
	void AddFillCircle(qreal x, qreal y, qreal radius, QColor& color);
	void ClearShapes();

	virtual SharedCaretState GetNextPosition(SharedCaretState& relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState& relativeState = SharedCaretState());
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const ShapeFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			ar << node->parent;
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

#endif
