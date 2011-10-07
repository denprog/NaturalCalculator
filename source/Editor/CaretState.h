#ifndef CARETSTATE_H
#define CARETSTATE_H

#include <vector>
#include <boost/shared_ptr.hpp>

using namespace std;

class FormulaNode;
class FormulaWnd;
class CurrentNode;

typedef vector<int> HierarchyPos;

class CaretState
{
public:
	CaretState();
	CaretState(FormulaNode* node, int pos);
	CaretState(const CaretState& source);
	~CaretState();
	
public:
	void SetState(CaretState& state);
	void SetToNode(FormulaNode* node, int pos);
	void SetToNodeBegin(FormulaNode* node);
	void SetToNodeEnd(FormulaNode* node);

	FormulaNode* GetNode();
	int GetPos();

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
	vector<int> positions;
	FormulaWnd* wnd;
};

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
