#include "ParserExpression.h"

ParserExpression::ParserExpression()
{
}

ParserExpression::ParserExpression(int _id) : id(_id)
{
}

ParserExpression::~ParserExpression()
{
}

bool ParserExpression::operator==(int _id)
{
	return id == _id;
}
