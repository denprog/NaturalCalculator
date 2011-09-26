#include "FormulaNodesCollection.h"
#include "FormulaNode.h"

FormulaNodesCollection::FormulaNodesCollection()
{
}

FormulaNodesCollection::~FormulaNodesCollection()
{
}

void FormulaNodesCollection::Add(FormulaNode* node)
{
	nodes.push_back(node);
}

void FormulaNodesCollection::Insert(FormulaNode* node, int pos)
{
	nodes.insert(nodes.begin() + pos, node);
}

void FormulaNodesCollection::CopyFrom(FormulaNodesCollection& source)
{
	for (NodesIter iter = source.nodes.begin(); iter < source.nodes.end(); ++iter)
		Add((*iter)->Clone());
}

void FormulaNodesCollection::Remove(int pos)
{
	nodes.erase(nodes.begin() + pos);
}

void FormulaNodesCollection::Clear()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		delete *iter;

	nodes.clear();
}

FormulaNodesCollection* FormulaNodesCollection::Clone()
{
	FormulaNodesCollection* res = new FormulaNodesCollection();
	
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		res->Add((*iter)->Clone());
	
	return res;
}

void FormulaNodesCollection::Remake()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		(*iter)->Remake();
}

void FormulaNodesCollection::Render()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		(*iter)->Render();
}

void FormulaNodesCollection::UpdateBoundingRect()
{
	for (NodesIter iter = nodes.begin(); iter < nodes.end(); ++iter)
		(*iter)->UpdateBoundingRect();
}

FormulaNode* FormulaNodesCollection::operator[](int pos) const
{
	assert(pos >= 0 && pos < (int)nodes.size());

	return nodes[pos];
}

int FormulaNodesCollection::GetPos(FormulaNode* node)
{
	for (int i = 0; i < (int)nodes.size(); ++i)
	{
		if (nodes[i] == node)
			return i;
	}
	
	return -1;
}
	
FormulaNode* FormulaNodesCollection::GetFirst()
{
	assert(nodes.size() > 0);
	
	return nodes[0];
}

FormulaNode* FormulaNodesCollection::GetLast()
{
	assert(nodes.size() > 0);
	
	return nodes[nodes.size() - 1];
}

int FormulaNodesCollection::Count()
{
	return (int)nodes.size();
}
