#include "Command.h"
#include "../FormulaNodes/FormulaNode.h"

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
	if (actionNode && actionNode->GetNode())
		command->nodeEvent["actionNode"] = actionNode;
	return action(caretState->GetNode(), command);
}

/**
 * Constructor.
 * @param _caretState State of the caret.
 * @param _doAction	The do action.
 * @param _nodeEvent The node event.
 */
Command::Command(SharedCaretState _caretState, CommandAction _doAction, NodeEvent _nodeEvent) : doAction(_doAction)
{
	beforeCaretState = SharedCaretState(_caretState->Dublicate());
	nodeEvent = _nodeEvent;
	afterCaretState = SharedCaretState(_caretState->Dublicate());
	nodeEvent["caretState"] = afterCaretState;
	nodeEvent["command"] = this;
}

/**
 * Destructor.
 */
Command::~Command()
{
	for (QMap<vector<int>, QMap<QString, boost::any> >::iterator iter = params.begin(); iter != params.end(); ++iter)
	{
		if (iter.value().contains("node"))
			delete any_cast<FormulaNode*>(iter.value()["node"]);
	}
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
	{
		//command must pass these parameters in case of successfull action
		undoAction = any_cast<CommandAction>(nodeEvent["undoAction"]);
		return true;
	}
	
	return false;
}

/**
 * Executes the undo action.
 * @return true if it succeeds, false if it fails.
 */
bool Command::UndoAction()
{
	//undo action
	return undoAction(this);
}

/**
 * Sets a parameter of this command.
 * @param [in,out] node The node, which needs this parameter.
 * @param name The name of the parameter.
 * @param param The parameter.
 */
void Command::SetParam(FormulaNode* node, const char* name, boost::any param)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	params[pos][name] = param;
}

/**
 * Gets a parameter.
 * @param [in,out] node The node, which stored this parameter.
 * @param name The name of the parameter.
 * @return The parameter.
 */
boost::any Command::GetParam(FormulaNode* node, const char* name)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	return params[pos][name];
}

/**
 * Query if this command contains a parameter.
 * @param [in,out] node The node, which may be stored this parameter.
 * @param name The name of the parameter.
 * @return true if it contains the parameter.
 */
bool Command::ContainsParam(FormulaNode* node, const char* name)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	return params.contains(pos) && params[pos].contains(name);
}

/**
 * Removes a parameter.
 * @param [in,out] node The node, which stored the parameter.
 * @param name The name of the parameter.
 */
void Command::RemoveParam(FormulaNode* node, const char* name)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	params[pos].remove(name);
}
