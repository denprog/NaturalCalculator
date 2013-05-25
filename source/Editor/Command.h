#ifndef COMMAND_H
#define COMMAND_H

#include <boost/function.hpp>
#include <QMap>
#include <boost/any.hpp>
#include "CaretState.h"

typedef QMap<QString, boost::any> NodeEvent;
typedef boost::function<bool (FormulaNode*, NodeEvent&)> CommandFunc;

/**
 * Command action.
 */
struct CommandAction
{
	CommandAction();
	CommandAction(FormulaNode* node, int pos, boost::function<bool (FormulaNode*, NodeEvent&)> _action);
	CommandAction(SharedCaretState _caretState, boost::function<bool (FormulaNode*, NodeEvent&)> _action);
	CommandAction(FormulaNode* node, int pos, FormulaNode* _actionNode, boost::function<bool (FormulaNode*, NodeEvent&)> _action);
	
	bool operator()(NodeEvent& nodeEvent);
	
	SharedCaretState caretState;	///< State of the caret
	SharedCaretState actionNode;	///< The action node
	boost::function<bool (FormulaNode*, NodeEvent&)> action; ///< The action's method
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
	
private:
	NodeEvent nodeEvent; //action parameters
	CommandAction doAction, undoAction; //functions to be called for do and undo actions
	SharedCaretState beforeCaretState, afterCaretState; //caret states before and after executing the command
	QMap<std::vector<int>, QMap<QString, boost::any> > params; //command parameters fills by nodes being operated
};

#endif
