#ifndef ROOTFORMULANODE_H
#define ROOTFORMULANODE_H

//#include <QApplication>
#include "../Main/NaturalCalculatorApp.h"
#include "GroupFormulaNode.h"
//#include <QGraphicsItemGroup>

/**
 * Root node for formula nodes.
 */
class RootFormulaNode : public GroupFormulaNode
{
public:
	RootFormulaNode();
	RootFormulaNode(FormulaNode* parent);
	RootFormulaNode(FormulaWnd* wnd);

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
	virtual FormulaNode* Clone(FormulaNode* p);
	
	virtual SharedCaretState GetFirstPosition();
	virtual SharedCaretState GetLastPosition();
	virtual SharedCaretState GetNextPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState relativeState = SharedCaretState());

public:
	virtual bool DoCreateEquationFormulaNode(Command* command);

	static RootFormulaNode* FromString(std::string str, FormulaWnd* wnd);
	
private:
	QGraphicsScene* scene;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const RootFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->parent;
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, RootFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)RootFormulaNode(parent);
		}
	}
}

#endif
