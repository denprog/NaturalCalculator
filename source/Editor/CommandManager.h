#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <deque>
#include <QString>
#include "Command.h"

class FormulaWnd;
class FormulaNode;

/**
 * Manager for commands.
 */
class CommandManager
{
public:
	CommandManager(FormulaWnd* _wnd);
	~CommandManager();
	
public:
	bool InsertNode(FormulaNode* node, CommandAction doAction, bool commit = true);
	bool InsertText(QString& text, CommandAction doAction, bool commit = true);
	bool InsertLine(bool commit = true);
	bool Remove(bool right, bool commit = true);
	
	bool DoAction(CommandAction doAction, NodeEvent nodeEvent, bool commit = true);
	
	bool Undo();
	bool Redo();
	void Reset();

private:
	void Rollback();
	
private:
	FormulaWnd* wnd;
	std::deque<Command*> undoCommands;
	std::deque<Command*> redoCommands;
	typedef std::deque<Command*>::iterator CommandsIter;
};

#endif
