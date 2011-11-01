#ifndef FORMULAWND_H
#define FORMULAWND_H

#include <QGraphicsView>
#include <vector>
#include <fstream>
#include "../FormulaNodes/DocumentFormulaNode.h"
#include "../FormulaNodes/PowerFormulaNode.h"
#include "../FormulaNodes/EquationFormulaNode.h"
#include "../FormulaNodes/FormulaNodesCollection.h"
#include "../Editor/Caret.h"
#include "../Editor/CommandManager.h"
#include "Settings.h"
#include "../ParserThread/ParserThread.h"

/**
 * Formula window. 
 */
class FormulaWnd : public QGraphicsView
{
	Q_OBJECT

public:
	FormulaWnd(QWidget *parent = 0);
	~FormulaWnd();

public:
	friend class Caret;
	friend class FormulaNode;
	friend class DocumentFormulaNode;
	friend class RootFormulaNode;
	friend class EquationFormulaNode;
	friend class ResultFormulaNode;
	
protected:
	virtual bool event(QEvent* e);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);

public:
	void New();
	bool Open(QString fileName);
	bool Save(QString fileName);
	void Undo();
	void Redo();
	
	void InsertNode(boost::function<bool (FormulaNode*, NodeEvent&)> action);
	void UpdateView();
	void EnsureVisible();
	
private:
	template<class Archive>
	void RegisterTypes(Archive& ar)
	{
		boost::serialization::void_cast_register<DocumentFormulaNode, FormulaNode>(static_cast<DocumentFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<RootFormulaNode, FormulaNode>(static_cast<RootFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<ShapeFormulaNode, FormulaNode>(static_cast<ShapeFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<GroupFormulaNode, FormulaNode>(static_cast<GroupFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<EmptyFormulaNode, FormulaNode>(static_cast<EmptyFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<TextFormulaNode, FormulaNode>(static_cast<TextFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<PlusFormulaNode, FormulaNode>(static_cast<PlusFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<MinusFormulaNode, FormulaNode>(static_cast<MinusFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<MultiplyFormulaNode, FormulaNode>(static_cast<MultiplyFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<DivisionFormulaNode, FormulaNode>(static_cast<DivisionFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<PowerFormulaNode, FormulaNode>(static_cast<PowerFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<SquareRootFormulaNode, FormulaNode>(static_cast<SquareRootFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<EquationFormulaNode, FormulaNode>(static_cast<EquationFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		
		ar.template register_type<FormulaNode>();
		ar.template register_type<DocumentFormulaNode>();
		ar.template register_type<RootFormulaNode>();
		ar.template register_type<ShapeFormulaNode>();
		ar.template register_type<GroupFormulaNode>();
		ar.template register_type<EmptyFormulaNode>();
		ar.template register_type<TextFormulaNode>();
		ar.template register_type<PlusFormulaNode>();
		ar.template register_type<MinusFormulaNode>();
		ar.template register_type<MultiplyFormulaNode>();
		ar.template register_type<DivisionFormulaNode>();
		ar.template register_type<PowerFormulaNode>();
		ar.template register_type<SquareRootFormulaNode>();
		ar.template register_type<EquationFormulaNode>();
	}

public:
	DocumentFormulaNode* documentNode;
	Caret* caret;
	static int updateEventId;
	ParserThread* parserThread;
	
private:
	QGraphicsScene* scene;
	CommandManager commandManager;
	Settings* settings;
	FormulaNode* mouseOverNode;
};

#endif // FORMULAWND_H
