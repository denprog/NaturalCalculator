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
#include "../Main/Settings.h"
#include "../ParserThread/ParserExpression.h"

using namespace std;
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
	virtual void ReplaceChild(FormulaNode* node, int pos);
	virtual int ChildrenCount();
	
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
	virtual void Render();
	
	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual void ParseStructure(QString& res);
#endif
	
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
	
	virtual bool DoCreatePlusFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreatePlusFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateMinusFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateMinusFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateMultiplyFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateMultiplyFormulaNode(NodeEvent& nodeEvent);
	
	virtual bool DoCreateDivisionFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateDivisionFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreatePowerFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreatePowerFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateSquareRootFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateSquareRootFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateLeftBraceFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateLeftBraceFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateRightBraceFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateRightBraceFormulaNode(NodeEvent& nodeEvent);
	
	virtual bool DoCreateEquationFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateEquationFormulaNode(NodeEvent& nodeEvent);
	
public:
	virtual void MakeContextMenu(QMenu* menu);
	bool MenuContainsAction(QMenu* menu, QString& action);

public slots:	
	virtual void OnCopy();
	virtual void OnPaste();
	virtual void OnCut();
		
public:
	/**
	 * Getter of the parent of this node.
	 * @return null The parent.
	 */
	FormulaNode* GetParent() const
	{
		return parent;
	}

	/**
	 * Getter of the child nodes.
	 * @return The child nodes.
	 */
	FormulaNodesCollection* GetChildNodes()
	{
		return childNodes;
	}

	/**
	 * Getter of the item.
	 * @return null if it fails, else the item.
	 */
	QGraphicsItem* GetItem()
	{
		return item;
	}
	
	/**
	 * Getter of the bounding rectangle.
	 * @return The bounding rectangle.
	 */
	QRectF& GetBoundingRect()
	{
		return boundingRect;
	}
	
	/**
	 * Getter of the window.
	 * @return The parent window.
	 */
	FormulaWnd* GetWnd() const
	{
		return wnd;
	}

	/**
	 * Getter of the baseline.
	 * @return The baseline.
	 */
	qreal GetBaseline()
	{
		return baseline;
	}
		
protected:
	FormulaNode* parent; ///< The parent node
	FormulaNodesCollection* childNodes; ///< The child nodes
	QRectF boundingRect; ///< The bounding rectangle
	QGraphicsItem* item;	///< The graphics item
	qreal baseline; ///< The baseline
	FormulaWnd* wnd; ///< The formula window
	FormulaNodeLevel level; ///< The node level
	Command* command;	
	Settings* settings;
	
#ifdef _DEBUG
	QString name;
#endif
};

#endif
