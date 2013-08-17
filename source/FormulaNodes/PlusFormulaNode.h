#ifndef PLUSFORMULANODE_H
#define PLUSFORMULANODE_H

#include "TextFormulaNode.h"

class PlusFormulaNode : public TextShapeFormulaNode
{
public:
	PlusFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	
private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);

	virtual void Parse(ParserString& expr);
	
#ifdef TEST
	virtual std::string ParseStructure();
#endif
	
	virtual std::string ToString();
	
	virtual FormulaNode* Clone(FormulaNode* p);
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const PlusFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, PlusFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)PlusFormulaNode(parent, parent->wnd);
		}
	}
}

#endif
