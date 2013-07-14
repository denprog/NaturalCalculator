#ifndef MULTIPLYFORMULANODE_H
#define MULTIPLYFORMULANODE_H

#include "ShapeFormulaNode.h"

class MultiplyFormulaNode : public ShapeFormulaNode
{
public:
	MultiplyFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~MultiplyFormulaNode();

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
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual void Parse(ParserString& expr);
	
#ifdef TEST
	void ParseStructure(QString& res);
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();
	
	virtual FormulaNode* Clone(FormulaNode* p);
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const MultiplyFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, MultiplyFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)MultiplyFormulaNode(parent, parent->wnd);
		}
	}
}

#endif
