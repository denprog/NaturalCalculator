#ifndef POWERFORMULANODE_H
#define POWERFORMULANODE_H

#include "CompoundFormulaNode.h"
#include "FormulaNodesCollection.h"

class PowerFormulaNode : public CompoundFormulaNode
{
public:
	PowerFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	PowerFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FormulaNode* _base, FormulaNode* _exponent);
	~PowerFormulaNode();

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
		InsertChild(right, 2);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	virtual void RemoveChildNodes();
	
	virtual void Remake();

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual void Parse(ParserString& expr);

#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();

	//command functions
	virtual bool DoRemoveItem(Command* command);

public:
	GroupFormulaNode *base, *exponent;
	
private:
	ShapeFormulaNode* shape;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const PowerFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, PowerFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)PowerFormulaNode(parent, parent->wnd);
		}
	}
}

#endif
