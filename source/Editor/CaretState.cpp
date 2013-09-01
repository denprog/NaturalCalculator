#include "CaretState.h"
#include "../FormulaNodes/FormulaNode.h"
#include "../FormulaNodes/TextFormulaNode.h"
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
 * Constructor.
 * @param [in] node The node being set the caret to.
 */
CaretState::CaretState(FormulaNode* node)
{
	if (node->parent == NULL)
		currentNode = new CurrentNode(node, 0);
	else
		currentNode = new CurrentNode(node->parent, node->parent->GetChildPos(node));
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
	if (node->type != TEXT_NODE && node->childNodes->Count() > pos && (*node)[pos]->type == TEXT_NODE)
		currentNode->SetNodePos((*node)[pos], 0);
	else
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
 * Gets the current node.
 * @return null if it fails, else the current node.
 */
FormulaNode* CaretState::GetCurrentNode()
{
	if (!currentNode)
		return NULL;
	FormulaNode* node = currentNode->GetNode();
	int pos = currentNode->GetPos();
	if (node->type != TEXT_NODE)
	{
		if (pos < node->childNodes->Count())
			return (*node)[pos];
		return (*node)[pos - 1];
	}
	return node;
}

QRectF CaretState::GetBounds()
{
	FormulaNode* node = currentNode->GetNode();
	return node->GetDocumentPosBounds(GetPos());
}

/**
 * Checks whether the caret state located on the node.
 * @param [in,out] node The node.
 * @return true if it succeeds, false if it fails.
 */
bool CaretState::CheckOnNode(FormulaNode* node)
{
	FormulaNode* n = GetNode();
	
	return n->childNodes->Count() > GetPos() ? (*n)[GetPos()] == node : false;
}

/**
 * Checks whether the caret state located inside the node.
 * @param [in,out] node The node.
 * @return true if it succeeds, false if it fails.
 */
bool CaretState::CheckInNode(FormulaNode* node)
{
	return currentNode->CheckInNode(node);
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
CaretPosition::CaretPosition() : wnd(NULL)
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
	if (!node)
		return;
	wnd = node->wnd;
	positions.clear();
	positions.push_back(pos);
	node->GetHierarchyPos(positions);
}

/**
 * Gets the current node.
 * @return The node.
 */
FormulaNode* CaretPosition::GetNode()
{
	FormulaNode* n = wnd->GetDocumentNode();
	for (int i = (int)positions.size() - 1; i > 0; --i)
	{
		if (!n || n->childNodes->Count() <= positions[i])
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
	if (positions.size() == 0)
		return -1;
	return positions[0];
}

bool CaretPosition::CheckInNode(FormulaNode* node)
{
	HierarchyPos pos;
	node->GetHierarchyPos(pos);
	
	if (positions.size() < pos.size())
		return false;
	for (int i = positions.size() - 1, j = pos.size() - 1; i >= 0 && j >= 0; --i, --j)
	{
		if (positions[i] != pos[j])
			return false;
	}
	
	return true;
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

bool CurrentNode::CheckInNode(FormulaNode* node)
{
	return position.CheckInNode(node);
}
