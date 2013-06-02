#ifndef COMMAND_H
#define COMMAND_H

#include <boost/function.hpp>
#include <QtCore/QMap>
#include <boost/any.hpp>
#include "CaretState.h"

class Command;

typedef QMap<QString, boost::any> NodeEvent;
typedef boost::function<bool (FormulaNode*, Command* command)> CommandFunction;

/**
 * Command action.
 */
struct CommandAction
{
	CommandAction();
	CommandAction(FormulaNode* node, int pos, CommandFunction _action);
	CommandAction(SharedCaretState _caretState, CommandFunction _action);
	CommandAction(FormulaNode* node, int pos, FormulaNode* _actionNode, CommandFunction _action);
	
	bool operator()(Command* command);
	
	SharedCaretState caretState;	///< State of the caret
	SharedCaretState actionNode;	///< The action node
	CommandFunction action; ///< The action's method
};

/**
 * Command.
 */
class Command
{
public:
	Command(SharedCaretState _caretState, CommandAction _doAction, NodeEvent nodeEvent);
	~Command();

public:
	friend class CommandManager;
		
public:
	bool DoAction();
	bool UndoAction();
	
	void SetParam(FormulaNode* node, const char* name, boost::any param);
	boost::any GetParam(FormulaNode* node, const char* name);
	bool ContainsParam(FormulaNode* node, const char* name);
	void RemoveParam(FormulaNode* node, const char* name);
	
public:
	NodeEvent nodeEvent; //action parameters
	
private:
	CommandAction doAction, undoAction; //functions to be called for do and undo actions
	SharedCaretState beforeCaretState, afterCaretState; //caret states before and after executing the command
	QMap<std::vector<int>, QMap<QString, boost::any> > params; //command parameters fills by nodes being operated
};

#endif
