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
	/**
	 * Default constructor.
	 */
	CommandAction()
	{
	}
	
	/**
	 * Constructor.
	 * @param [in] node	The parent node.
	 * @param pos	The caret position.
	 * @param [in] _action The action's method.
	 */
	CommandAction(FormulaNode* node, int pos, boost::function<bool (FormulaNode*, NodeEvent&)> _action) : 
		caretState(new CaretState(node, pos)), action(_action)
	{
	}

	/**
	 * Constructor.
	 * @param _caretState	State of the caret.
	 * @param [in] _action The action's method.
	 */
	CommandAction(SharedCaretState _caretState, boost::function<bool (FormulaNode*, NodeEvent&)> _action) : 
		caretState(_caretState->Dublicate()), action(_action)
	{
	}
	
	/**
	 * The functor for executing an action.
	 * @param nodeEvent The node event
	 */
	bool operator()(NodeEvent& nodeEvent)
	{
		return action(caretState->GetNode(), nodeEvent);
	}
	
	SharedCaretState caretState;	///< State of the caret
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
	QMap<vector<int>, QMap<QString, boost::any> > params; //command parameters fills by nodes being operated
};

#endif
