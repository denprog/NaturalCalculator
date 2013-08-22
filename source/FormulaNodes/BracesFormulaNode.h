#ifndef BRACESFORMULANODE_H
#define BRACESFORMULANODE_H

#include "CompoundFormulaNode.h"
#include "FormulaNodesCollection.h"

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
	virtual void AddChild(FormulaNode* node);
	virtual void RemoveChildNodes();

	virtual void Remake();

	virtual bool CanInsert();

	virtual FormulaNode* GetExpression(int pos) const;
	virtual void ShowShape(int pos, bool show);
	virtual bool IsShapeVisible(int pos) const;

	virtual FormulaNode* Clone(FormulaNode* p);

	SharedCaretState GetLastPosition();

	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();

	//command functions
	virtual bool DoRemoveItem(Command* command);
	
	virtual SharedCaretState GetNextPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState relativeState = SharedCaretState());

public:
	GroupFormulaNode* inside;
	
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
			FormulaNode* parent = node->parent;
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
			::new (node)BracesFormulaNode(parent, parent->wnd, left, right);
		}
	}
}

#endif
