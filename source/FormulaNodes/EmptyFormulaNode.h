#ifndef EMPTYFORMULANODE_H
#define EMPTYFORMULANODE_H

#include "ShapeFormulaNode.h"

class EmptyFormulaNode : public ShapeFormulaNode
{
public:
	EmptyFormulaNode(FormulaNode* _parent);
	virtual ~EmptyFormulaNode();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar << boost::serialization::base_object<ShapeFormulaNode>(*this);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar >> boost::serialization::base_object<ShapeFormulaNode>(*this);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual SharedCaretState GetNextPosition(SharedCaretState& relativeState = SharedCaretState());
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const EmptyFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->GetParent();
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, EmptyFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->GetParent();
			ar >> parent;
			::new (node)EmptyFormulaNode(parent);
		}
	}
}

#endif
