#ifndef FORMULAWND_H
#define FORMULAWND_H

#include <QGraphicsView>
#include <vector>
#include <fstream>
#include "../FormulaNodes/DocumentFormulaNode.h"
#include "../FormulaNodes/EquationFormulaNode.h"
#include "../FormulaNodes/FormulaNodesCollection.h"
#include "../Editor/Caret.h"
#include "../Editor/CommandManager.h"
#include "Settings.h"
#include "../ParserThread/ParserThread.h"

/**
 * @class	FormulaWnd
 * @brief	Formula window. 
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
	
protected:
	virtual bool event(QEvent* e);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);

public:
	bool Open(QString fileName);
	bool Save(QString fileName);
	void Undo();
	void Redo();
	
private:
	void UpdateView();
	
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
		boost::serialization::void_cast_register<DivisionFormulaNode, FormulaNode>(static_cast<DivisionFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		boost::serialization::void_cast_register<EquationFormulaNode, FormulaNode>(static_cast<EquationFormulaNode*>(NULL), static_cast<FormulaNode*>(NULL));
		
		ar.template register_type<FormulaNode>();
		ar.template register_type<DocumentFormulaNode>();
		ar.template register_type<RootFormulaNode>();
		ar.template register_type<ShapeFormulaNode>();
		ar.template register_type<GroupFormulaNode>();
		ar.template register_type<EmptyFormulaNode>();
		ar.template register_type<TextFormulaNode>();
		ar.template register_type<DivisionFormulaNode>();
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
};

#endif // FORMULAWND_H
