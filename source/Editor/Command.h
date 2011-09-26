#ifndef COMMAND_H
#define COMMAND_H

#include <boost/function.hpp>
#include <QMap>
//#include <QVariant>
#include <boost/any.hpp>
#include "CaretState.h"

typedef QMap<QString, boost::any> NodeEvent;
typedef boost::function<bool (FormulaNode*, NodeEvent&)> CommandFunc;

//#define TemplateAction template<class T> boost::function<bool (T*, NodeEvent&)>;

//struct CommandAction
//{
//	template<class NodeType>
//	CommandAction(FormulaNode* node, int pos, boost::function<bool (NodeType*, NodeEvent&)>& func)
//	{
//	}
//};

struct CommandAction
{
	CommandAction()
	{
	}
	
	CommandAction(FormulaNode* node, int pos, boost::function<bool (FormulaNode*, NodeEvent&)> _action) : 
		caretState(new CaretState(node, pos)), action(_action)
	{
	}

	CommandAction(SharedCaretState _caretState, boost::function<bool (FormulaNode*, NodeEvent&)> _action) : 
		caretState(_caretState->Dublicate()), action(_action)
	{
	}
	
	//CommandAction(const CommandAction& source) : caretState(new CaretState(*source.caretState)), action(source.action)
	//{
	//}
	
	bool operator()(NodeEvent& nodeEvent)
	{
		return action(caretState->GetNode(), nodeEvent);
	}
	
	//void operator=(const boost::function<bool (FormulaNode*, NodeEvent&)> _action)
	//{
	//	action = _action;
	//}
	
	SharedCaretState caretState;
	boost::function<bool (FormulaNode*, NodeEvent&)> action;
};

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
