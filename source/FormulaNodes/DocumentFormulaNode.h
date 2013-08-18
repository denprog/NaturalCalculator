#ifndef DOCUMENTFORMULANODE_H
#define DOCUMENTFORMULANODE_H

#include "../Main/NaturalCalculatorApp.h"
#include "FormulaNode.h"
#include "RootFormulaNode.h"
#include "EmptyFormulaNode.h"
#include <new>

//#ifdef new
//#undef new
//#endif

/**
 * Document of formula nodes.
 */
class DocumentFormulaNode : public FormulaNode
{
public:
	DocumentFormulaNode();
	DocumentFormulaNode(FormulaWnd* wnd);
	
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
	virtual FormulaNode* Clone(FormulaNode* p);
	
	void AddLine();
	
	void Clear();

	virtual SharedCaretState GetFirstPosition();
	virtual SharedCaretState GetLastPosition();
	virtual SharedCaretState GetNextPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState relativeState = SharedCaretState());

	//commands
	virtual bool DoInsertLine(Command* command);
	
#ifdef TEST
	virtual std::string ParseStructure();
#endif

	static bool FromString(std::string& str, DocumentFormulaNode* node);
	virtual std::string ToString();
	
public:
	QGraphicsScene* scene;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const DocumentFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			int pos = ((NaturalCalculatorApp*)qApp)->GetFormulaWndPos(node->wnd);
			//store window position in the app's array
			ar << pos;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, DocumentFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			int pos;
			ar >> pos;
			FormulaWnd* wnd = ((NaturalCalculatorApp*)qApp)->GetFormulaWnd(pos);
			::new (node)DocumentFormulaNode(wnd);
		}
	}
}

#endif
