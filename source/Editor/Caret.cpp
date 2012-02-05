#include "Caret.h"
#include "../FormulaNodes/FormulaNode.h"
#include "../Main/FormulaWnd.h"

/**
 * Constructor.
 * @param [in] _wnd The formula window.
 */
Caret::Caret(FormulaWnd* _wnd)
{
	wnd = _wnd;
	caretShape = new QGraphicsItemGroup();
	wnd->scene->addItem(caretShape);
	
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));
	timer->start(500);
}

/**
 * Destructor.
 */
Caret::~Caret()
{
	qDeleteAll(caretShape->childItems());
	caretShape->childItems().clear();
	delete timer;
}

/**
 * Renders the caret.
 */
void Caret::Render()
{
	currentState->GetNode()->RenderCaret(currentState->GetPos(), currentState->GetPos());
	timer->start(500);
	caretShape->setVisible(true);
}

/**
 * Sets a caret state.
 * @param state The caret state.
 */
void Caret::SetState(SharedCaretState state)
{
	currentState = state;
}
	
/**
 * Sets caret to node and its position.
 * @param [in,out] node The node.
 * @param pos	The position.
 */
void Caret::SetToNode(FormulaNode* node, int pos)
{
	currentState->SetToNode(node, pos);
}

/**
 * Sets the caret to the node begin.
 * @param [in] node The node.
 */
void Caret::SetToNodeBegin(FormulaNode* node)
{
	currentState = node->GetFirstPosition();
}

/**
 * Sets the caret to the node end.
 * @param [in,out] node The node.
 */
void Caret::SetToNodeEnd(FormulaNode* node)
{
}
	
/**
 * Moves the caret left.
 */
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

/**
 * Moves the caret right.
 */
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
	FormulaNode* n = currentState->GetNode();
	if (!n)
		return;
	
	SharedCaretState c = n->GetLineBegin(currentState);
	if (!c)
		return;
	
	SetState(c);
	Render();
}

void Caret::MoveEnd()
{
	FormulaNode* n = currentState->GetNode();
	if (!n)
		return;
	
	SharedCaretState c = n->GetLineEnd(currentState);
	if (!c)
		return;
	
	SetState(c);
	Render();
}

void Caret::TimerUpdate()
{
	caretShape->setVisible(!caretShape->isVisible());
}
