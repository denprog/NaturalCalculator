#ifndef FORMULANODE_H
#define FORMULANODE_H

#include <cassert>
#include <QRect>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <vector>
#include <QObject>
#include <boost/any.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include "../Editor/Caret.h"
#include "../Editor/CaretState.h"
#include "../Editor/Command.h"
#include "../ParserThread/ParserExpression.h"

using boost::any_cast;

class FormulaWnd;
class FormulaNodesCollection;

class QMenu;

enum FormulaNodeLevel
{
	NORMAL_LEVEL = 1,
	LESS_LEVEL, 
	STILL_LESS_LEVEL
};

enum NodeType
{
	EMPTY_NODE,
	DOCUMENT_NODE, 
	ROOT_NODE,
	TEXT_NODE,
	SHAPE_NODE,
	GROUP_NODE,
	COMPOUND_NODE,
	PLUS_NODE,
	MINUS_NODE,
	MULTIPLY_NODE, 
	DIVISION_NODE, 
	POWER_NODE, 
	SQUARE_ROOT_NODE, 
	BRACES_NODE, 
	RESULT_NODE, 
	RESULT_ITEM_NODE, 
	EQUATION_NODE, 
	COMMA_NODE
};

class Settings;

/**
 * Formula node.
 */
class FormulaNode : public QObject
{
	Q_OBJECT
	
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
	virtual void RemoveChild(FormulaNode* node);
	virtual void RemoveChildNodes();
	virtual void ReplaceChild(FormulaNode* node, int pos);
	virtual void Normalize();
	
	virtual int ChildrenCount();
	
	virtual bool CanInsert(int pos);
	virtual bool CanRemove(int pos);
	
	virtual FormulaNode* GetExpression(int pos) const;
	virtual void ShowShape(int pos, bool show);
	virtual bool IsShapeVisible(int pos) const;
	
	virtual FormulaNode* Clone(FormulaNode* p);
	
	FormulaNode* operator[](int pos) const;
	
	virtual void Remake();
	virtual void Update();
	virtual void UpdateBoundingRect();
	
	virtual FormulaNodeLevel GetLesserLevel();
	virtual FormulaNodeLevel GetGreaterLevel();
	virtual void SetLevel(FormulaNodeLevel _level);
	
	virtual void Move(int x, int y);

	virtual void GetHierarchyPos(HierarchyPos& positions) const;
	
	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual std::string ParseStructure();
#endif
	
	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();
	
	int GetChildPos(const FormulaNode* node) const;
	bool IsChild(const FormulaNode* node);
	int GetFirstLevelChildPos(FormulaNode* node);
	
	virtual QRectF GetDocumentBounds();
	virtual QRectF GetDocumentPosBounds(int pos);
	
	virtual int GetNearestPos(qreal x, qreal y);

	virtual bool IsEmptySymbol();
	
	//caret functions
	
	virtual SharedCaretState GetFirstPosition();
	virtual SharedCaretState GetLastPosition();
	virtual SharedCaretState GetNextPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState relativeState = SharedCaretState());
	virtual SharedCaretState GetLineBegin(SharedCaretState& relativeState);
	virtual SharedCaretState GetLineEnd(SharedCaretState& relativeState);

	virtual bool CanSetCaret();
	
	virtual void RenderCaret(const int pos, const int anchor);
	
	//command functions
	
	virtual bool DoInsertNode(Command* command);
	virtual bool DoInsertText(Command* command);
	
	virtual bool DoInsertLine(Command* command);
	virtual bool UndoInsertLine(Command* command);
	
	virtual bool DoRemoveItem(Command* command);
	
	virtual bool DoCreatePlusFormulaNode(Command* command);
	virtual bool DoCreateMinusFormulaNode(Command* command);
	virtual bool DoCreateMultiplyFormulaNode(Command* command);
	virtual bool DoCreateDivisionFormulaNode(Command* command);
	virtual bool DoCreatePowerFormulaNode(Command* command);
	virtual bool DoCreateSquareRootFormulaNode(Command* command);
	virtual bool DoCreateLeftBraceFormulaNode(Command* command);
	virtual bool DoCreateRightBraceFormulaNode(Command* command);
	
	virtual bool DoCreateEquationFormulaNode(Command* command);
	virtual bool UndoCreateEquationFormulaNode(Command* command);
	
public:
	virtual void MakeContextMenu(QMenu* menu);
	bool MenuContainsAction(QMenu* menu, QString action);

public slots:	
	virtual void OnCopy();
	virtual void OnPaste();
	virtual void OnCut();
		
public:
	NodeType type;
	FormulaNode* parent; ///< The parent node
	FormulaNodesCollection* childNodes; ///< The child nodes
	QRectF boundingRect; ///< The bounding rectangle
	QGraphicsItem* item;	///< The graphics item
	qreal baseline; ///< The baseline
	FormulaWnd* wnd; ///< The formula window
	FormulaNodeLevel level; ///< The node level
	Command* command;	
	Settings* settings;
};

#endif
