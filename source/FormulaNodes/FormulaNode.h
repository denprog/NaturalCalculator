#ifndef FORMULANODE_H
#define FORMULANODE_H

#include <cassert>
#include <QRect>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <vector>
#include <boost/any.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include "../Editor/Caret.h"
#include "../Editor/CaretState.h"
#include "../Editor/Command.h"
#include "../Main/Settings.h"
#include "../ParserThread/ParserExpression.h"

using namespace std;
using boost::any_cast;

class FormulaWnd;
class FormulaNodesCollection;

enum FormulaNodeLevel
{
	NORMAL_LEVEL = 1,
	LESS_LEVEL, 
	STILL_LESS_LEVEL
};

/**
 * Base class for formula nodes
 * @class
 */
class FormulaNode
{
public:
	FormulaNode();
	FormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	virtual ~FormulaNode();

public:
	friend class CaretState;
	friend class CaretPosition;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar << childNodes;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		if (childNodes)
		{
			delete childNodes;
			childNodes = NULL;
		}
		ar >> childNodes;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	//child functions
	
	virtual void AddChild(FormulaNode* node);
	virtual void InsertChild(FormulaNode* node, int pos);
	virtual void MoveChild(FormulaNode* node, int pos);
	virtual void RemoveChild(int pos);
	
	virtual FormulaNode* Clone();
	
	FormulaNode* operator[](int pos);
	
	virtual void Remake();
	virtual void Update();
	virtual void UpdateBoundingRect();
	virtual void SetLevel();
	virtual void Move(int x, int y);
	virtual void SetSize();

	virtual void GetHierarchyPos(HierarchyPos& positions);
	virtual void Render();
	
	virtual void Parse(ParserExpression& expr);
	
	int GetChildPos(FormulaNode* node);
	bool IsChild(const FormulaNode* node);
	int GetFirstLevelChildPos(FormulaNode* node);
	
	virtual QRectF GetDocumentBounds();
	virtual QRectF GetDocumentPosBounds(int pos);

	virtual bool IsEmptySymbol();
	
	//caret functions
	
	virtual SharedCaretState GetFirstPosition();
	virtual SharedCaretState GetLastPosition();
	virtual SharedCaretState GetNextPosition(SharedCaretState& relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState& relativeState = SharedCaretState());

	virtual bool CanSetCaret();
	
	virtual void RenderCaret(const int pos, const int anchor);
	
	//command functions
	
	virtual bool DoInsertNode(NodeEvent& nodeEvent);
	virtual bool UndoInsertNode(NodeEvent& nodeEvent);
	virtual bool DoInsertText(NodeEvent& nodeEvent);
	virtual bool UndoInsertText(NodeEvent& nodeEvent);
	
	virtual bool DoInsertLine(NodeEvent& nodeEvent);
	virtual bool UndoInsertLine(NodeEvent& nodeEvent);
	
	virtual bool DoRemoveItem(NodeEvent& nodeEvent);
	virtual bool UndoRemoveItem(NodeEvent& nodeEvent);
	
	//template<class T>
	//bool DoCreateNode(NodeEvent& nodeEvent);
	
	virtual bool DoCreatePlusFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreatePlusFormulaNode(NodeEvent& nodeEvent);
	virtual bool DoCreateDivisionFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateDivisionFormulaNode(NodeEvent& nodeEvent);
	virtual bool DoCreateEquationFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateEquationFormulaNode(NodeEvent& nodeEvent);

public:
	FormulaNode* parent;
	FormulaNodesCollection* childNodes;
	QRectF boundingRect;
	QGraphicsItem* item;
	int baseline;
	FormulaWnd* wnd;
	
protected:
	Settings* settings;
#ifdef _DEBUG
	QString name;
#endif

private:
	Command* command;	
};

//#if defined(WIN32) && defined(_DEBUG)
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#define new DEBUG_NEW
//#endif

#endif
