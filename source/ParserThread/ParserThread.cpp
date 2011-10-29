#include "ParserThread.h"
#include "../Main/FormulaWnd.h"
#include <QCoreApplication>

/**
 * Constructor.
 * @param [in] _wnd The formula window.
 */
ParserThread::ParserThread(FormulaWnd* _wnd) : wnd(_wnd)
{
	exit = false;
	thread = new boost::thread(SolvingThread(this));
}

/**
 * Destructor.
 */
ParserThread::~ParserThread()
{
	exit = true;
	expressionsReady.notify_one();
	thread->join();
	delete thread;
}

/**
 * Adds an expression to be solved.
 * @param [in,out] expr The expression.
 */
void ParserThread::AddExpression(ParserExpressionVariant& expr)
{
	boost::mutex::scoped_lock lock(expressionsMutex);
	expressionsToSolve.push_back(expr);
	expressionsReady.notify_one();
}

/**
 * Removes a expression.
 * @param [in,out] expr The expression.
 */
void ParserThread::RemoveExpression(ParserExpressionVariant& expr)
{
	boost::mutex::scoped_lock lock(expressionsMutex);
	//remove the element from the both vectors
	ParserExpressionVariantIter it = find(expressionsToSolve.begin(), expressionsToSolve.end(), expr);
	if (it != expressionsToSolve.end())
		expressionsToSolve.erase(it);
	it = find(solvedExpressions.begin(), solvedExpressions.end(), expr);
	if (it != solvedExpressions.end())
		solvedExpressions.erase(it);
}

/**
 * Gets a solved expression.
 * @param [in,out] expr The expression.
 * @return true if it succeeds, false if it fails.
 */
bool ParserThread::GetSolvedExpression(ParserExpressionVariant& expr)
{
	boost::mutex::scoped_lock lock(expressionsMutex);

	ParserExpressionVariantIter it = find(solvedExpressions.begin(), solvedExpressions.end(), expr);
	if (it != solvedExpressions.end())
	{
		expr = *it;
		solvedExpressions.erase(it);
		return true;
	}
	
	return false;
}

/**
 * Constructor.
 * @param [in,out] _parserThread The parser thread.
 */
ParserThread::SolvingThread::SolvingThread(ParserThread* _parserThread)
{
	parserThread = _parserThread;
}

/**
 * Destructor.
 */
ParserThread::SolvingThread::~SolvingThread()
{
}

/**
 * The solving thread
 */
void ParserThread::SolvingThread::operator()()
{
	using BigNumbersParser::Parser;
	
	boost::mutex mut;

	//init the parsers
	realParser = new Parser<Real>(10);
	integerParser = new Parser<Integer>(10);
	rationalParser = new Parser<Rational>(10);
	
	parsers.push_back(realParser);
	parsers.push_back(integerParser);
	parsers.push_back(rationalParser);

	//the solving loop
	while (!parserThread->exit)
	{
		boost::unique_lock<boost::mutex> lock(mut);

		//get the expressions
		parserThread->expressionsMutex.lock();
		expressions = parserThread->expressionsToSolve;
		parserThread->expressionsToSolve.clear();
		parserThread->expressionsMutex.unlock();

		//wait until the expressions are ready
		while (expressions.size() == 0 && !parserThread->exit)
		{
			parserThread->expressionsReady.wait(lock);

			parserThread->expressionsMutex.lock();
			expressions = parserThread->expressionsToSolve;
			parserThread->expressionsToSolve.clear();
			parserThread->expressionsMutex.unlock();
		}
		
		//solve the expressions
		for (int i = 0, j; i < (int)expressions.size(); ++i)
		{
			ParserExpressionVariant& expr = expressions[i];
			
			//solve the expression
			boost::apply_visitor(*this, expr.var);

			//push the solved result
			parserThread->expressionsMutex.lock();
			ParserExpressionVariantIter it = find(parserThread->solvedExpressions.begin(), parserThread->solvedExpressions.end(), expr);
			if (it != parserThread->solvedExpressions.end())
				*it = expr;
			else
				parserThread->solvedExpressions.push_back(expr);
			parserThread->expressionsMutex.unlock();
			
			//say what the expression was solved
			QCoreApplication::postEvent(parserThread->wnd, new QEvent((QEvent::Type)FormulaWnd::updateEventId));
		}
	}

	//clearing
	delete realParser;
	delete integerParser;
	delete rationalParser;
}

/**
 * The solving visitor for RealParserExpression.
 */
void ParserThread::SolvingThread::operator()(RealParserExpression const& expr) const
{
	try
	{
		((RealParserExpression&)expr).result = realParser->Parse((string&)expr.expression.expression, expr.precision);
	}
	catch (ParserException e)
	{
		((RealParserExpression&)expr).exception = e;
	}
}

/**
 * The solving visitor for IntegerParserExpression.
 */
void ParserThread::SolvingThread::operator()(IntegerParserExpression const& expr) const
{
	try
	{
		((IntegerParserExpression&)expr).result = integerParser->Parse((string&)expr.expression.expression);
	}
	catch (ParserException e)
	{
		((IntegerParserExpression&)expr).exception = e;
	}
}

/**
 * The solving visitor for RationalParserExpression.
 */
void ParserThread::SolvingThread::operator()(RationalParserExpression const& expr) const
{
	try
	{
		((RationalParserExpression&)expr).result = rationalParser->Parse((string&)expr.expression.expression);
	}
	catch (ParserException e)
	{
		((RationalParserExpression&)expr).exception = e;
	}
}

/**
 * The solving visitor for AutoParserExpression.
 */
void ParserThread::SolvingThread::operator()(AutoParserExpression const& expr) const
{
	int j;
	AutoParserExpression& expression = (AutoParserExpression&)expr;
	ParserException firstException;

	//try to solve the expression with the given parsers in their order
	for (j = 0; j < (int)parsers.size(); ++j)
	{
		try
		{
			try
			{
				Parser<Real>* p = boost::get<Parser<Real>*>(parsers[j]);
				expression.result = p->Parse(expression.expression.expression, expression.precision);
				break;
			}
			catch (boost::bad_get)
			{
			}

			try
			{
				Parser<Integer>* p = boost::get<Parser<Integer>*>(parsers[j]);
				expression.result = p->Parse(expression.expression.expression);
				break;
			}
			catch (boost::bad_get)
			{
			}

			try
			{
				Parser<Rational>* p = boost::get<Parser<Rational>*>(parsers[j]);
				expression.result = p->Parse(expression.expression.expression);
				break;
			}
			catch (boost::bad_get)
			{
			}
		}
		catch (ParserException e)
		{
			if (j == 0)
				firstException = e;
		}
	}

	if (j == (int)parsers.size())
		expression.exception = firstException;
}
