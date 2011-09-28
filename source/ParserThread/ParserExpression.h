#ifndef PARSEREXPRESSION_H
#define PARSEREXPRESSION_H

#include <string>
#include <boost/variant.hpp>
#include "Integer.h"
#include "Real.h"
#include "Rational.h"

using namespace std;
using namespace BigNumbersParser;

struct ParserExpression
{
	ParserExpression();
	ParserExpression(int _id);
	~ParserExpression();
	
	bool operator==(int _id);
	
	int id;
	string expression;
	int precision;
	boost::variant<Integer, Real, Rational> result;
	ParserException exception;
};

#endif
