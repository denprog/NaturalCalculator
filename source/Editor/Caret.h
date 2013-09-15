#ifndef CARET_H
#define CARET_H

#include <QGraphicsItemGroup>
#include <QObject>
#include <QTimer>
#include "CaretState.h"

class FormulaNode;
class RootFormulaNode;

/**
 * Caret.
 */
class Caret : public QObject
{
	Q_OBJECT
	
public:
	Caret(FormulaWnd* _wnd);
	~Caret();
	
public:
	void Render();
	
	void SetState(SharedCaretState state, bool updateXPos = true);
	void SetToNode(FormulaNode* node, int pos);
	void SetToNodeBegin(FormulaNode* node);
	void SetToNodeEnd(FormulaNode* node);
	
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void MoveHome();
	void MoveEnd();

public slots:
	void TimerUpdate();
	
public:
	QGraphicsItemGroup* caretShape; ///< The caret group shape
	SharedCaretState currentState; ///< The current caret state
	int xPos;
	
private:
	FormulaWnd* wnd;
	QTimer* timer;
};

#endif
