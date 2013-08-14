#include "Command.h"
#include "../FormulaNodes/FormulaNode.h"
#include "../FormulaNodes/FormulaNodesCollection.h"

/**
 * Default constructor.
 */
CommandAction::CommandAction()
{
}

/**
 * Constructor.
 * @param [in] node	The parent node.
 * @param pos	The caret position.
 * @param [in] _action The action's method.
 */
CommandAction::CommandAction(FormulaNode* node, int pos, CommandFunction _action) : 
	caretState(new CaretState(node, pos)), action(_action)
{
}

/**
 * Constructor.
 * @param _caretState	State of the caret.
 * @param [in] _action The action's method.
 */
CommandAction::CommandAction(SharedCaretState _caretState, CommandFunction _action) : 
	caretState(_caretState->Dublicate()), action(_action)
{
}

/**
 * Constructor.
 * @param [in] node	The parent node.
 * @param pos	The position.
 * @param [in] _actionNode The node being used in the action.
 * @param [in] _action [in] The action funcion.
 */
CommandAction::CommandAction(FormulaNode* node, int pos, FormulaNode* _actionNode, CommandFunction _action) : 
	caretState(new CaretState(node, pos)), actionNode(new CaretState(_actionNode)), action(_action)
{
}

/**
 * The functor for executing an action.
 * @param nodeEvent The node event
 */
bool CommandAction::operator()(Command* command)
{
	return action(caretState->GetNode(), command);
}

/**
 * Constructor.
 * @param _caretState State of the caret.
 * @param _doAction	The do action.
 * @param _nodeEvent The node event.
 */
Command::Command(SharedCaretState _caretState, CommandAction _doAction, NodeEvent _nodeEvent) : nodeEvent(_nodeEvent), doAction(_doAction)
{
	beforeCaretState = SharedCaretState(_caretState->Dublicate());
	afterCaretState = SharedCaretState(_caretState->Dublicate());
}

/**
 * Destructor.
 */
Command::~Command()
{
}

/**
 * Executes the action operation.
 * @return true if it succeeds, false if it fails.
 */
bool Command::DoAction()
{
	afterCaretState->SetState(*beforeCaretState);
	
	//do the action
	if (doAction(this))
		return true;
	
	return false;
}

/**
 * Executes the undo action.
 * @return true if it succeeds, false if it fails.
 */
bool Command::UndoAction()
{
	std::string::iterator b = savedNode.begin();
	std::string::iterator e = savedNode.end();
	FormulaNode* n = savedNodePos->GetCurrentNode();
	n->RemoveChildNodes();
	FormulaNode::FromString(b, e, n);
	n->Normalize();
	return true;
}

void Command::SaveNodeState(FormulaNode* node)
{
	while (node->type == BRACES_NODE || node->type == DIVISION_NODE || node->type == SQUARE_ROOT_NODE || 
		node->type == NTH_ROOT_NODE || node->type == EQUATION_NODE || node->type == POWER_NODE)
		node = node->parent;
	savedNode = node->ToString();
	savedNodePos = SharedCaretState(new CaretState(node));
}
