#ifndef GROUPFORMULANODE_H
#define GROUPFORMULANODE_H

#include "FormulaNode.h"

class ShapeFormulaNode;

/**
 * Group formula node.
 */
class GroupFormulaNode : public FormulaNode
{
public:
	GroupFormulaNode();
	GroupFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~GroupFormulaNode();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar << boost::serialization::base_object<FormulaNode>(*this);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar >> boost::serialization::base_object<FormulaNode>(*this);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	virtual void Remake();
	virtual void Update();
	//virtual void UpdateBoundingRect();
	//virtual void Move(int x, int y);

	virtual FormulaNode* Clone(FormulaNode* p);

#ifdef TEST
	virtual void ParseStructure(QString& res);
#endif

	virtual ShapeFormulaNode* AddShapeNode();
	virtual ShapeFormulaNode* InsertShapeNode(int pos);

	virtual SharedCaretState GetFirstPosition();
	virtual SharedCaretState GetLastPosition();
	virtual SharedCaretState GetNextPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetLineBegin(SharedCaretState& relativeState);
	virtual SharedCaretState GetLineEnd(SharedCaretState& relativeState);
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const GroupFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, GroupFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)GroupFormulaNode(parent, parent->wnd);
		}
	}
}

#endif
