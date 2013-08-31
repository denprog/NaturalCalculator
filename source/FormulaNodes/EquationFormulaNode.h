#ifndef EQUATIONFORMULANODE_H
#define EQUATIONFORMULANODE_H

#include "CompoundFormulaNode.h"
#include "ResultFormulaNode.h"
#include "FormulaNodesCollection.h"

/**
 * Equation formula node.
 */
class EquationFormulaNode : public CompoundFormulaNode
{
	Q_OBJECT
	
public:
	EquationFormulaNode();
	EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		FormulaNode* left = (*childNodes)[0];
		ar << left;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		FormulaNode* left;
		ar >> left;
		InsertChild(left, 0);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	virtual void RemoveChildNodes();
	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();
	
	//command functions
	virtual bool DoInsertText(Command* command);
	virtual bool DoRemoveItem(Command* command);

public:
	GroupFormulaNode* left;
	GroupFormulaNode* right;
	HierarchyPos errorPos;
	
private:
	ShapeFormulaNode* shape;
	ResultFormulaNode* result;
	string lastExpression;
	QGraphicsPathItem* errorItem;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const EquationFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
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
