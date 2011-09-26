#ifndef CARET_H
#define CARET_H

#include "CaretState.h"
#include <QGraphicsItemGroup>

class FormulaNode;

class Caret
{
public:
	Caret(FormulaWnd* _wnd);
	~Caret();
	
public:
	void Render();
	
	void SetState(SharedCaretState state);
	void SetToNodeBegin(FormulaNode* node);
	void SetToNodeEnd(FormulaNode* node);
	
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void MoveHome();
	void MoveEnd();

public:
	QGraphicsItemGroup* caretShape;
	SharedCaretState currentState;
	
private:
	FormulaWnd* wnd;
};

#endif
