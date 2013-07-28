#include "CommandManager.h"
#include "../Main/FormulaWnd.h"

CommandManager::CommandManager(FormulaWnd* _wnd)
{
	wnd = _wnd;
}

CommandManager::~CommandManager()
{
	Reset();
}

bool CommandManager::InsertNode(FormulaNode* node, CommandAction doAction, bool commit)
{
	SharedCaretState caretState = SharedCaretState(wnd->GetCaret()->currentState->Dublicate());
	if (!caretState->GetNode()->CanInsert(caretState->GetPos()))
		return false;
	
	NodeEvent nodeEvent;
	nodeEvent["node"] = node;
	
	Command* command = new Command(caretState, doAction, nodeEvent);
	if (command->DoAction())
	{
		undoCommands.push_front(command);
	}
	else
	{
		delete command;
		Rollback();
		return false;
	}
	
	//set new caret state
	wnd->GetCaret()->SetState(undoCommands[0]->afterCaretState);
	
	if (commit)
	{
		//push the delimiter
		undoCommands.push_front(NULL);
	}

	return true;
}

bool CommandManager::InsertText(QString& text, CommandAction doAction, bool commit)
{
	SharedCaretState caretState = SharedCaretState(wnd->GetCaret()->currentState->Dublicate());
	if (!caretState->GetNode()->CanInsert(caretState->GetPos()))
		return false;
	
	NodeEvent nodeEvent;
	nodeEvent["text"] = text;

	return DoAction(doAction, nodeEvent, commit);
}

bool CommandManager::InsertLine(bool commit)
{
	SharedCaretState caretState = SharedCaretState(wnd->GetCaret()->currentState->Dublicate());
	if (!caretState->GetNode()->CanInsert(caretState->GetPos()))
		return false;
	
	NodeEvent nodeEvent;
	CommandAction doAction(caretState, &FormulaNode::DoInsertLine);
	
	return DoAction(doAction, nodeEvent, commit);
}

bool CommandManager::Remove(bool right, bool commit)
{
	SharedCaretState caretState = SharedCaretState(wnd->GetCaret()->currentState->Dublicate());
	if (!caretState->GetNode()->CanRemove(caretState->GetPos()))
		return false;
	
	NodeEvent nodeEvent;
	nodeEvent["right"] = right;

	return DoAction(CommandAction(caretState, &FormulaNode::DoRemoveItem), nodeEvent, commit);	
}

bool CommandManager::DoAction(CommandAction doAction, NodeEvent nodeEvent, bool commit)
{
	SharedCaretState caretState = SharedCaretState(wnd->GetCaret()->currentState->Dublicate());
	
	Command* command = new Command(caretState, doAction, nodeEvent);
	//do action
	if (command->DoAction())
	{
		undoCommands.push_front(command);
		
		for (CommandsIter iter = redoCommands.begin(); iter != redoCommands.end(); ++iter)
		{
			if (*iter)
				delete *iter;
		}
		redoCommands.clear();
	}
	else
	{
		delete command;
		Rollback();
		return false;
	}

	//set new caret state
	wnd->GetCaret()->SetState(undoCommands[0]->afterCaretState);
	
	if (commit)
	{
		//push the delimiter
		undoCommands.push_front(NULL);
	}

	return true;
}

bool CommandManager::Undo()
{
	if (undoCommands.size() == 0)
		return false;
	
	//extract the delimiters
	while (undoCommands.size() != 0 && undoCommands[0] == NULL)
		undoCommands.pop_front();

	SharedCaretState c;
		
	while (undoCommands.size() != 0 && undoCommands[0])
	{
		Command* command = undoCommands[0];
		
		//undo the action
		if (command->UndoAction())
		{
			redoCommands.push_front(*undoCommands.begin());
			c = redoCommands[0]->beforeCaretState;
			undoCommands.pop_front();
		}
		else
		{
			undoCommands.pop_front();
			Rollback();
			return false;
		}
	}
	
	//push the delimiter
	redoCommands.push_front(NULL);

	if (c)
	{
		//set new caret state
		wnd->GetCaret()->SetState(c);
	}
	
	return true;
}

bool CommandManager::Redo()
{
	if (redoCommands.size() == 0)
		return false;
	
	while (redoCommands.size() != 0 && redoCommands[0] == NULL)
		redoCommands.pop_front();

	SharedCaretState c;
	
	while (redoCommands.size() != 0 && redoCommands[0])
	{
		Command* command = redoCommands[0];

		//redo the action		
		command->DoAction();
		undoCommands.push_front(*redoCommands.begin());
		c = redoCommands[0]->afterCaretState;
		redoCommands.pop_front();
	}
	
	//push the delimiter
	undoCommands.push_front(NULL);
	
	if (c)
	{
		//set new caret state
		wnd->GetCaret()->SetState(c);
	}
	
	return true;
}

void CommandManager::Reset()
{
	for (CommandsIter iter = undoCommands.begin(); iter != undoCommands.end(); ++iter)
	{
		if (*iter)
			delete *iter;
	}
	undoCommands.clear();
	
	for (CommandsIter iter = redoCommands.begin(); iter != redoCommands.end(); ++iter)
	{
		if (*iter)
			delete *iter;
	}
	redoCommands.clear();
}

void CommandManager::Rollback()
{
	while (undoCommands.size() != 0 && undoCommands[0])
	{
		Undo();
	}
}
