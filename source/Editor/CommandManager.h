#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <deque>
#include <QString>
#include "Command.h"

using namespace std;

class FormulaWnd;
class FormulaNode;

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

	//template<class T>
	//bool Insert(FormulaNode* node, boost::function<bool (T*, NodeEvent&)> doAction)
	//{
	//	return true;
	//}
	
	bool Undo();
	bool Redo();
	void Reset();

private:
	void Rollback();
	
private:
	FormulaWnd* wnd;
	deque<Command*> undoCommands;
	deque<Command*> redoCommands;
	typedef deque<Command*>::iterator CommandsIter;
};

#endif
