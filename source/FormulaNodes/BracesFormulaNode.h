#ifndef BRACESFORMULANODE_H
#define BRACESFORMULANODE_H

#include "CompoundFormulaNode.h"

/**
 * Braces formula node.
 */
class BracesFormulaNode : public CompoundFormulaNode
{
public:
	BracesFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, bool left, bool right);
	virtual ~BracesFormulaNode();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		FormulaNode* expr = (*childNodes)[leftShape ? 1 : 0];
		ar << expr;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		FormulaNode *expr;
		ar >> expr;
		InsertChild(expr, leftShape ? 1 : 0);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* GetExpression(int pos) const;
	virtual void ShowShape(int pos, bool show);
	virtual bool IsShapeVisible(int pos) const;

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual void Parse(ParserString& expr);
	
	//virtual void RenderCaret(const int pos, const int anchor);
	
	//command functions
	virtual bool DoRemoveItem(NodeEvent& nodeEvent);

//public:
//	bool GetLeftShape()
//	{
//		return leftShape != NULL;
//	}
//	
//	bool GetRightShape()
//	{
//		return rightShape != NULL;
//	}
//	
//	void SetLeftShape(bool setShape);
//	void SetRightShape(bool setShape);
	
private:
	ShapeFormulaNode *leftShape, *rightShape;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const BracesFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->GetParent();
			ar << parent;
			bool left = node->IsShapeVisible(0);
			bool right = node->IsShapeVisible(1);
			ar << left;
			ar << right;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, BracesFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			bool left, right;
			ar >> left;
			ar >> right;
			::new (node)BracesFormulaNode(parent, parent->GetWnd(), left, right);
		}
	}
}

#endif
