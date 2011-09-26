#include "Caret.h"
#include "../FormulaNodes/FormulaNode.h"
#include "../Main/FormulaWnd.h"

Caret::Caret(FormulaWnd* _wnd)
{
	//currentState = NULL;
	wnd = _wnd;
	caretShape = new QGraphicsItemGroup();
	wnd->scene->addItem(caretShape);
}

Caret::~Caret()
{
	qDeleteAll(caretShape->childItems());
	caretShape->childItems().clear();
}

void Caret::Render()
{
	currentState->GetNode()->RenderCaret(currentState->GetPos(), currentState->GetPos());
}

void Caret::SetState(SharedCaretState state)
{
	currentState = state;
}
	
void Caret::SetToNodeBegin(FormulaNode* node)
{
	currentState = node->GetFirstPosition();
}

void Caret::SetToNodeEnd(FormulaNode* node)
{
}
	
void Caret::MoveLeft()
{
	FormulaNode* n = currentState->GetNode();
	if (!n)
		return;
	
	SharedCaretState c = n->GetPreviousPosition(currentState);
	if (!c)
		return;
	
	SetState(c);
	Render();
}

void Caret::MoveRight()
{
	FormulaNode* n = currentState->GetNode();
	if (!n)
		return;
	
	SharedCaretState c = n->GetNextPosition(currentState);
	if (!c)
		return;
	
	SetState(c);
	Render();
}

void Caret::MoveUp()
{
}

void Caret::MoveDown()
{
}

void Caret::MoveHome()
{
}

void Caret::MoveEnd()
{
}
