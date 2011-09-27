#ifndef EQUATIONFORMULANODE_H
#define EQUATIONFORMULANODE_H

#include "CompoundFormulaNode.h"

class EquationFormulaNode : public CompoundFormulaNode
{
public:
	EquationFormulaNode();
	EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~EquationFormulaNode();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		//ar << boost::serialization::base_object<CompoundFormulaNode>(*this);
		FormulaNode* left = (*childNodes)[0];
		ar << left;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		//ar >> boost::serialization::base_object<CompoundFormulaNode>(*this);
		FormulaNode* left;
		ar >> left;
		InsertChild(left, 0);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone();

private:
	ShapeFormulaNode* shape;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const EquationFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			ar << node->parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, EquationFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)EquationFormulaNode(parent, parent->wnd);
		}
	}
}

#endif
