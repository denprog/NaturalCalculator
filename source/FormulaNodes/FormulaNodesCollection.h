#ifndef FORMULANODESCOLLECTION_H
#define FORMULANODESCOLLECTION_H

#include <vector>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include "FormulaNode.h"

using namespace std;

/**
 * Collection of formula nodes.
 */
class FormulaNodesCollection
{
public:
	FormulaNodesCollection();
	~FormulaNodesCollection();

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		int c = ((FormulaNodesCollection&)*this).Count();
		ar << c;
		for (int i = 0; i < c; ++i)
		{
			FormulaNode* n = (*this)[i];
			ar << n;
		}
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		FormulaNode* node;
		int c;
		ar >> c;
		
		for (int i = 0; i < c; ++i)
		{
			ar >> node;
			Add(node);
		}
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
public:
	void Add(FormulaNode* node);
	void Insert(FormulaNode* node, int pos);
	void CopyFrom(FormulaNodesCollection& source, FormulaNode* parent);
	void Remove(int pos);
	void Clear();

	FormulaNodesCollection* Clone(FormulaNode* parent);
	
	void Remake();
	void UpdateBoundingRect();

	FormulaNode* operator[](int pos) const;
	
	int GetPos(const FormulaNode* node) const;

	FormulaNode* GetFirst();
	FormulaNode* GetLast();
			
	int Count();
	
private:
	vector<FormulaNode*> nodes;
	
	typedef vector<FormulaNode*>::iterator NodesIter;
};

#endif
