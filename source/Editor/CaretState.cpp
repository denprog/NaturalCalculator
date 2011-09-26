#include "CaretState.h"
#include "../FormulaNodes/FormulaNode.h"
#include "../Main/FormulaWnd.h"

//CaretState

CaretState::CaretState()
{
	currentNode = NULL;
}

CaretState::CaretState(FormulaNode* node, int pos)
{
	currentNode = new CurrentNode(node, pos);
}

CaretState::CaretState(const CaretState& source)
{
	currentNode = new CurrentNode(((CaretState&)source).GetNode(), ((CaretState&)source).GetPos());
}

CaretState::~CaretState()
{
	delete currentNode;
}

void CaretState::SetState(CaretState& state)
{
	currentNode->SetNodePos(state.GetNode(), state.GetPos());
}

void CaretState::SetToNode(FormulaNode* node, int pos)
{
	currentNode->SetNodePos(node, pos);
}
	
void CaretState::SetToNodeBegin(FormulaNode* node)
{
}

void CaretState::SetToNodeEnd(FormulaNode* node)
{
}

FormulaNode* CaretState::GetNode()
{
	if (!currentNode)
		return NULL;
	return currentNode->GetNode();
}

int CaretState::GetPos()
{
	if (!currentNode)
		return -1;
	return currentNode->GetPos();
}

//void CaretState::operator=(const CaretState& source)
//{
//	if (currentNode)
//		delete currentNode;
//	if (((CaretState&)source).Empty())
//		currentNode = NULL;
//	else
//		currentNode = new CurrentNode(((CaretState&)source).GetNode(), ((CaretState&)source).GetPos());
//}

//bool CaretState::Empty()
//{
//	return (currentNode == NULL) || (currentNode->GetNode() == NULL);
//}

bool CaretState::CheckOnNode(FormulaNode* node)
{
	FormulaNode* n = GetNode();
	
	return n->childNodes->Count() > GetPos() ? (*n)[GetPos()] == node : NULL;
}

bool CaretState::CheckInNode(FormulaNode* node)
{
	return GetNode() == node || node->IsChild(GetNode());
}

bool CaretState::CheckAtLast(FormulaNode* node)
{
	FormulaNode* parent = GetNode();
	if (node && parent != node)
		return false;
	
	return GetPos() != 0 && GetPos() == parent->childNodes->Count();
}

bool CaretState::operator==(const CaretState& state) const
{
	return ((CaretState*)this)->GetNode() == ((CaretState&)state).GetNode() && ((CaretState*)this)->GetPos() == ((CaretState&)state).GetPos();
}

bool CaretState::operator!=(const CaretState& state) const
{
	return !(operator==(state));
}

CaretState* CaretState::Dublicate()
{
	return new CaretState(GetNode(), GetPos());
}

//CaretPosition

CaretPosition::CaretPosition()
{
}

CaretPosition::~CaretPosition()
{
}
	
void CaretPosition::SetNodePos(FormulaNode* node, int pos)
{
	wnd = node->wnd;
	positions.clear();
	positions.push_back(pos);
	node->GetHierarchyPos(positions);
}

FormulaNode* CaretPosition::GetNode()
{
	FormulaNode* n = wnd->documentNode;
	for (int i = (int)positions.size() - 1; i > 0; --i)
	{
		if (!n)
			return NULL;
		n = (*n)[positions[i]];
	}
	
	return n;
}

int CaretPosition::GetPos()
{
	return positions[0];
}

//CurrentNode

CurrentNode::CurrentNode(FormulaNode* node, int pos)
{
	position.SetNodePos(node, pos);
	anchor = 0;
}

CurrentNode::~CurrentNode()
{
}

void CurrentNode::SetNodePos(FormulaNode* node, int pos)
{
	position.SetNodePos(node, pos);
}
	
FormulaNode* CurrentNode::GetNode()
{
	return position.GetNode();
}

int CurrentNode::GetPos()
{
	return position.GetPos();
}
