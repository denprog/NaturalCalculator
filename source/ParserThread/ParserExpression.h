#ifndef PARSEREXPRESSION_H
#define PARSEREXPRESSION_H

#include <string>
#include <boost/variant.hpp>
#include "Integer.h"
#include "Real.h"
#include "Rational.h"
#include "../Editor/CaretState.h"

using namespace std;
using namespace BigNumbersParser;

class FormulaNode;

struct ParserExpression
{
	ParserExpression();
	ParserExpression(FormulaNode* node, int _precision);
	~ParserExpression();
	
	void Add(char* expr, FormulaNode* node);
	void Add(string& expr, FormulaNode* node);
	
	bool operator==(const HierarchyPos& _id);
	
	struct AnnotationPos
	{
		AnnotationPos(int _pos, int _length, HierarchyPos _hierarchyPos) : pos(_pos), length(_length), hierarchyPos(_hierarchyPos)
		{
		}
		
		int pos;
		int length;
		HierarchyPos hierarchyPos;
	};
	
	HierarchyPos pos; //position of the node, which created this object
	string expression; //expression to be solved
	vector<AnnotationPos> annotation; //for having possibility of finding error's position
	int precision; //current precision
	boost::variant<Integer, Real, Rational> result; //variant of results
	ParserException exception; //a possible solving exception
};

#endif
