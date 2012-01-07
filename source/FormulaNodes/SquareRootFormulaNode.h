#ifndef SQUAREROOTFORMULANODE_H
#define SQUAREROOTFORMULANODE_H

#include "CompoundFormulaNode.h"
#include "ShapeFormulaNode.h"

/**
 * Square root formula node.
 */
class SquareRootFormulaNode : public CompoundFormulaNode
{
public:
	SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~SquareRootFormulaNode();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		FormulaNode* right = (*childNodes)[1];
		ar << right;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		FormulaNode *right;
		ar >> right;
		InsertChild(right, 1);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual void Parse(ParserString& expr);
	
	virtual void RenderCaret(const int pos, const int anchor);
	
	//command functions
	virtual bool DoRemoveItem(NodeEvent& nodeEvent);

private:
	ShapeFormulaNode* shape;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const SquareRootFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->GetParent();
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, SquareRootFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)SquareRootFormulaNode(parent, parent->GetWnd());
		}
	}
}

#endif
