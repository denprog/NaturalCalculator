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
	virtual ~Command();

public:
	friend class CommandManager;
	
public:
	virtual bool DoAction();
	virtual bool UndoAction();
	
	virtual void SaveNodeState(FormulaNode* node);

public:
	NodeEvent nodeEvent; //action parameters
	SharedCaretState beforeCaretState, afterCaretState; //caret states before and after executing the command
	
protected:
	CommandAction doAction;
	std::string savedNode;
	SharedCaretState savedNodePos;
};

class DocumentCommand : public Command
{
public:
	DocumentCommand(SharedCaretState _caretState, CommandAction _doAction, NodeEvent nodeEvent, FormulaWnd* _wnd);

public:
	virtual bool UndoAction();
	
	virtual void SaveNodeState(FormulaNode* node);
	
protected:
	FormulaWnd* wnd;
};

#endif
