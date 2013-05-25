#ifndef CARETSTATE_H
#define CARETSTATE_H

#include <vector>
#include <boost/shared_ptr.hpp>

class FormulaNode;
class FormulaWnd;
class CurrentNode;

/**
 * Defines hierarchy position of a node, which is an array of node positions in their collections towards the root node.
 */
typedef std::vector<int> HierarchyPos;

/**
 * Caret state.
 */
class CaretState
{
public:
	CaretState();
	CaretState(FormulaNode* node, int pos);
	CaretState(FormulaNode* node);
	CaretState(const CaretState& source);
	~CaretState();
	
public:
	void SetState(CaretState& state);
	void SetToNode(FormulaNode* node, int pos);
	void SetToNodeBegin(FormulaNode* node);
	void SetToNodeEnd(FormulaNode* node);

	FormulaNode* GetNode();
	int GetPos();
	FormulaNode* GetCurrentNode();

	//bool Empty();
	
	bool CheckOnNode(FormulaNode* node);
	bool CheckInNode(FormulaNode* node);
	bool CheckAtLast(FormulaNode* node);

	CaretState* Dublicate();
			
public:
	bool operator==(const CaretState& state) const;
	bool operator!=(const CaretState& state) const;
	
public:
	//void operator=(const CaretState& source);
	
private:
	CurrentNode* currentNode;
};

/**
 * Caret position.
 */
class CaretPosition
{
public:
	CaretPosition();
	~CaretPosition();
	
public:
	void SetNodePos(FormulaNode* node, int pos);
	FormulaNode* GetNode();
	int GetPos();
	
private:
	std::vector<int> positions; ///< The caret positions
	FormulaWnd* wnd;
};

/**
 * Current node.
 */
class CurrentNode
{
public:
	CurrentNode(FormulaNode* node, int pos);
	~CurrentNode();
	
public:
	void SetNodePos(FormulaNode* node, int pos);
	FormulaNode* GetNode();
	int GetPos();
	
private:
	CaretPosition position;
	int anchor;
};

typedef boost::shared_ptr<CaretState> SharedCaretState;

#endif
