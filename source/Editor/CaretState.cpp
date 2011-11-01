#include "CaretState.h"
#include "../FormulaNodes/FormulaNode.h"
#include "../Main/FormulaWnd.h"

//CaretState

/**
 * Default constructor.
 */
CaretState::CaretState()
{
	currentNode = NULL;
}

/**
 * Constructor.
 * @param [in] node The node containing the caret state.
 * @param pos The position in the node.
 */
CaretState::CaretState(FormulaNode* node, int pos)
{
	currentNode = new CurrentNode(node, pos);
}

/**
 * Copy constructor.
 * @param source Source for the caret state.
 */
CaretState::CaretState(const CaretState& source)
{
	currentNode = new CurrentNode(((CaretState&)source).GetNode(), ((CaretState&)source).GetPos());
}

/**
 * Destructor.
 */
CaretState::~CaretState()
{
	delete currentNode;
}

/**
 * Sets a state from another caret state.
 * @param [in] state The caret state.
 */
void CaretState::SetState(CaretState& state)
{
	currentNode->SetNodePos(state.GetNode(), state.GetPos());
}

/**
 * Sets the caret state to a node and a position.
 * @param [in] node The node.
 * @param pos The position.
 */
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

/**
 * Gets the node from the caret state.
 * @return null if it fails, else the node.
 */
FormulaNode* CaretState::GetNode()
{
	if (!currentNode)
		return NULL;
	return currentNode->GetNode();
}

/**
 * Gets the position.
 * @return The position if the currentNode exists, else -1.
 */
int CaretState::GetPos()
{
	if (!currentNode)
		return -1;
	return currentNode->GetPos();
}

/**
 * Checks whether the caret state located on the node.
 * @param [in,out] node The node.
 * @return true if it succeeds, false if it fails.
 */
bool CaretState::CheckOnNode(FormulaNode* node)
{
	FormulaNode* n = GetNode();
	
	return n->childNodes->Count() > GetPos() ? (*n)[GetPos()] == node : NULL;
}

/**
 * Checks whether the caret state located inside the node.
 * @param [in,out] node The node.
 * @return true if it succeeds, false if it fails.
 */
bool CaretState::CheckInNode(FormulaNode* node)
{
	return GetNode() == node || node->IsChild(GetNode());
}

/**
 * Checks whether the caret state located at the last position of the node.
 * @param [in,out] node The node.
 * @return true if it succeeds, false if it fails.
 */
bool CaretState::CheckAtLast(FormulaNode* node)
{
	FormulaNode* parent = GetNode();
	if (node && parent != node)
		return false;
	
	return GetPos() != 0 && GetPos() == parent->childNodes->Count();
}

/**
 * == casting operator.
 * @param state The caret state.
 * @return The result of the operation.
 */
bool CaretState::operator==(const CaretState& state) const
{
	return ((CaretState*)this)->GetNode() == ((CaretState&)state).GetNode() && ((CaretState*)this)->GetPos() == ((CaretState&)state).GetPos();
}

/**
 * != casting operator.
 * @param state The caret state.
 * @return The result of the operation.
 */
bool CaretState::operator!=(const CaretState& state) const
{
	return !(operator==(state));
}

/**
 * Gets the dublicate.
 * @return null if it fails, else.
 */
CaretState* CaretState::Dublicate()
{
	return new CaretState(GetNode(), GetPos());
}

//CaretPosition

/**
 * Default constructor.
 */
CaretPosition::CaretPosition()
{
}

/**
 * Destructor.
 */
CaretPosition::~CaretPosition()
{
}
	
/**
 * Sets a node position.
 * @param [in] node The node.
 * @param pos The position.
 */
void CaretPosition::SetNodePos(FormulaNode* node, int pos)
{
	wnd = node->wnd;
	positions.clear();
	positions.push_back(pos);
	node->GetHierarchyPos(positions);
}

/**
 * Gets the current node.
 * @return null if it fails, else the node.
 */
FormulaNode* CaretPosition::GetNode()
{
	FormulaNode* n = wnd->GetDocumentNode();
	for (int i = (int)positions.size() - 1; i > 0; --i)
	{
		if (!n)
			return NULL;
		n = (*n)[positions[i]];
	}
	
	return n;
}

/**
 * Gets the position in the current node.
 * @return The position.
 */
int CaretPosition::GetPos()
{
	return positions[0];
}

//CurrentNode

/**
 * Constructor.
 * @param [in,out] node The node.
 * @param pos	The position.
 */
CurrentNode::CurrentNode(FormulaNode* node, int pos)
{
	position.SetNodePos(node, pos);
	anchor = 0;
}

/**
 * Destructor.
 */
CurrentNode::~CurrentNode()
{
}

/**
 * Sets a node position.
 * @param [in,out] node The node.
 * @param pos	The position.
 */
void CurrentNode::SetNodePos(FormulaNode* node, int pos)
{
	position.SetNodePos(node, pos);
}
	
/**
 * Gets the current node.
 * @return null if it fails, else the node.
 */
FormulaNode* CurrentNode::GetNode()
{
	return position.GetNode();
}

/**
 * Gets the current position.
 * @return The position.
 */
int CurrentNode::GetPos()
{
	return position.GetPos();
}
