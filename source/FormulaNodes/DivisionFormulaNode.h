#ifndef DIVISIONFORMULANODE_H
#define DIVISIONFORMULANODE_H

#include "CompoundFormulaNode.h"
#include "FormulaNodesCollection.h"

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
		FormulaNode* left = (*childNodes)[0];
		FormulaNode* right = (*childNodes)[2];
		ar << left;
		ar << right;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		FormulaNode *left, *right;
		ar >> left;
		ar >> right;
		InsertChild(left, 0);
		InsertChild(right, 1);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	virtual void AddChild(FormulaNode* node);
	virtual void InsertChild(FormulaNode* node, int pos);
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual bool CanInsert(int pos);

	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual void ParseStructure(QString& res);
#endif

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual SharedCaretState GetNextPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState relativeState = SharedCaretState());

	virtual void RenderCaret(const int pos, const int anchor);

	//command functions
	virtual bool DoRemoveItem(Command* command);

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
			FormulaNode* parent = node->GetParent();
			ar << parent;
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
