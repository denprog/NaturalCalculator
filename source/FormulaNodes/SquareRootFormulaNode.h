#ifndef SQUAREROOTFORMULANODE_H
#define SQUAREROOTFORMULANODE_H

#include "CompoundFormulaNode.h"
#include "ShapeFormulaNode.h"
#include "FormulaNodesCollection.h"

/**
 * Square root formula node.
 */
class SquareRootFormulaNode : public CompoundFormulaNode
{
public:
	SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	SquareRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FormulaNode* _radicand);

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
	virtual void RemoveChildNodes();

	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual bool CanInsert();

	virtual FormulaNode* GetExpression(int pos) const;
	virtual void ShowShape(int pos, bool show);
	virtual bool IsShapeVisible(int pos) const;

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual std::string ParseStructure();
#endif
	
	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();
	
	virtual void RenderCaret(const int pos, const int anchor);
	
	//command functions
	virtual bool DoRemoveItem(Command* command);

public:
	GroupFormulaNode* radicand;
	
protected:
	ShapeFormulaNode* shape;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const SquareRootFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, SquareRootFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)SquareRootFormulaNode(parent, parent->wnd);
		}
	}
}

#endif
