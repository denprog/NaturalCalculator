#include "Command.h"
#include "../FormulaNodes/FormulaNode.h"

Command::Command(SharedCaretState _caretState, CommandAction _doAction, NodeEvent _nodeEvent) : doAction(_doAction)
{
	//doAction = _doAction;
	beforeCaretState = SharedCaretState(_caretState->Dublicate());
	nodeEvent = _nodeEvent;
	afterCaretState = SharedCaretState(_caretState->Dublicate());
	nodeEvent["caretState"] = afterCaretState;
	nodeEvent["command"] = this;
}

Command::~Command()
{
}

bool Command::DoAction()
{
	//FormulaNode* node = beforeCaretState->GetNode();
	afterCaretState->SetState(*beforeCaretState);
	
	//do action
	if (doAction(nodeEvent))
	{
		//command must pass these parameters in case of successfull action
		undoAction = any_cast<CommandAction>(nodeEvent["undoAction"]);
		//afterCaretState = any_cast<SharedCaretState>(nodeEvent["caretState"]);
		//afterCaretState = beforeCaretState;
		//undoActionCaretState = any_cast<SharedCaretState>(nodeEvent["undoActionCaretState"]);
		return true;
	}
	
	return false;
}

bool Command::UndoAction()
{
	//FormulaNode* node = undoActionCaretState->GetNode();
	
	//undo action
	if (undoAction(nodeEvent))
	{
		//caretState = SharedCaretState(any_cast<SharedCaretState>(nodeEvent["caretState"])->Dublicate());
		return true;
	}
	
	return false;
}

void Command::SetParam(FormulaNode* node, const char* name, boost::any param)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	params[pos][name] = param;
}

boost::any Command::GetParam(FormulaNode* node, const char* name)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	return params[pos][name];
}

bool Command::ContainsParam(FormulaNode* node, const char* name)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	return params.contains(pos) && params[pos].contains(name);
}

void Command::RemoveParam(FormulaNode* node, const char* name)
{
	vector<int> pos;
	node->GetHierarchyPos(pos);
	
	params[pos].remove(name);
}
