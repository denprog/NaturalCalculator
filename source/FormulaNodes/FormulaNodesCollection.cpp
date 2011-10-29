#include "FormulaNodesCollection.h"
#include "FormulaNode.h"

/**
 * Default constructor.
 */
FormulaNodesCollection::FormulaNodesCollection()
{
}

/**
 * Destructor.
 */
FormulaNodesCollection::~FormulaNodesCollection()
{
}

/**
 * Adds a node.
 * @param [in] node FormulaNode* to add.
 */
void FormulaNodesCollection::Add(FormulaNode* node)
{
	nodes.push_back(node);
}

/**
 * Inserts a node at a position in the collection.
 * @param [in,out] node The node.
 * @param pos	The position.
 */
void FormulaNodesCollection::Insert(FormulaNode* node, int pos)
{
	nodes.insert(nodes.begin() + pos, node);
}

/**
 * Copies from the source.
 * @param [in,out] source Source collection.
 */
void FormulaNodesCollection::CopyFrom(FormulaNodesCollection& source)
{
	for (NodesIter iter = source.nodes.begin(); iter < source.nodes.end(); ++iter)
		Add((*iter)->Clone());
}

/**
 * Removes an element at the given pos.
 * @param pos The position to remove.
 */
void FormulaNodesCollection::Remove(int pos)
{
	nodes.erase(nodes.begin() + pos);
}

/**
 * Clears this object to its blank state.
 */
void FormulaNodesCollection::Clear()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		delete *iter;

	nodes.clear();
}

/**
 * Makes a deep copy of this object.
 * @return A copy of this object.
 */
FormulaNodesCollection* FormulaNodesCollection::Clone()
{
	FormulaNodesCollection* res = new FormulaNodesCollection();
	
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		res->Add((*iter)->Clone());
	
	return res;
}

/**
 * Remakes all the nodes.
 */
void FormulaNodesCollection::Remake()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		(*iter)->Remake();
}

/**
 * Renders all the nodes.
 */
void FormulaNodesCollection::Render()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		(*iter)->Render();
}

/**
 * Updates the bounding rectangle of the nodes.
 */
void FormulaNodesCollection::UpdateBoundingRect()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		(*iter)->UpdateBoundingRect();
}

/**
 * [] casting operator.
 * @param pos The position.
 * @return The result node.
 */
FormulaNode* FormulaNodesCollection::operator[](int pos) const
{
	assert(pos >= 0 && pos < (int)nodes.size());

	return nodes[pos];
}

/**
 * Gets a position of a node.
 * @param [in,out] node The node.
 * @return The position.
 */
int FormulaNodesCollection::GetPos(FormulaNode* node)
{
	for (int i = 0; i < (int)nodes.size(); ++i)
	{
		if (nodes[i] == node)
			return i;
	}
	
	return -1;
}
	
/**
 * Gets the first node.
 * @return The first node.
 */
FormulaNode* FormulaNodesCollection::GetFirst()
{
	assert(nodes.size() > 0);
	
	return nodes[0];
}

/**
 * Gets the last node.
 * @return The last node.
 */
FormulaNode* FormulaNodesCollection::GetLast()
{
	assert(nodes.size() > 0);
	
	return nodes[nodes.size() - 1];
}

/**
 * Gets the count.
 * @return Number of elements.
 */
int FormulaNodesCollection::Count()
{
	return (int)nodes.size();
}
