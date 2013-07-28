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
	virtual ~EquationFormulaNode();

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
	virtual void ParseStructure(QString& res);
#endif

	//command functions
	virtual bool DoInsertText(Command* command);

public:
	virtual void MakeContextMenu(QMenu* menu);
	
public slots:
	virtual void OnAddAutoResult();
	virtual void OnAddRealResult();
	virtual void OnAddIntegerResult();
	virtual void OnAddRationalResult();

public:
	GroupFormulaNode* left;

private:
	ShapeFormulaNode* shape;
	ResultFormulaNode* resNode;
	string lastExpression;
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
