#ifndef DIVISIONFORMULANODE_H
#define DIVISIONFORMULANODE_H

#include "CompoundFormulaNode.h"

class DivisionFormulaNode : public CompoundFormulaNode
{
public:
	DivisionFormulaNode();
	DivisionFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~DivisionFormulaNode();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar << boost::serialization::base_object<CompoundFormulaNode>(*this);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar >> boost::serialization::base_object<CompoundFormulaNode>(*this);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	virtual void AddChild(FormulaNode* node);
	virtual void InsertChild(FormulaNode* node, int pos);
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual void Parse(ParserExpression& expr);

	virtual FormulaNode* Clone();

	virtual SharedCaretState GetNextPosition(SharedCaretState& relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState& relativeState = SharedCaretState());

	virtual void RenderCaret(const int pos, const int anchor);

private:
	ShapeFormulaNode* shape;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const DivisionFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			ar << node->parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, DivisionFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)DivisionFormulaNode(parent, parent->wnd);
		}
	}
}

#endif
