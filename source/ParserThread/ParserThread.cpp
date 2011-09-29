#include "ParserThread.h"
#include "../Main/FormulaWnd.h"
#include <QCoreApplication>

ParserThread::ParserThread(FormulaWnd* _wnd) : wnd(_wnd)
{
	exit = false;
	thread = new boost::thread(SolvingThread(this));
}

ParserThread::~ParserThread()
{
	exit = true;
	expressionsReady.notify_one();
	thread->join();
	delete thread;
}

void ParserThread::AddExpression(ParserExpression& expr)
{
	boost::mutex::scoped_lock lock(expressionsMutex);
	expressionsToSolve.push_back(expr);
	expressionsReady.notify_one();
}

void ParserThread::RemoveExpression(HierarchyPos& id)
{
	boost::mutex::scoped_lock lock(expressionsMutex);
	//remove the element from the both vectors
	vector<ParserExpression>::iterator it = find(expressionsToSolve.begin(), expressionsToSolve.end(), id);
	if (it != expressionsToSolve.end())
		expressionsToSolve.erase(it);
	it = find(solvedExpressions.begin(), solvedExpressions.end(), id);
	if (it != solvedExpressions.end())
		solvedExpressions.erase(it);
}

bool ParserThread::GetSolvedExpression(HierarchyPos& pos, ParserExpression& expr)
{
	boost::mutex::scoped_lock lock(expressionsMutex);

	vector<ParserExpression>::iterator it = find(solvedExpressions.begin(), solvedExpressions.end(), pos);
	if (it != solvedExpressions.end())
	{
		expr = *it;
		solvedExpressions.erase(it);
		return true;
	}
	
	return false;
}

ParserThread::SolvingThread::SolvingThread(ParserThread* _parserThread) //: integerParser(10), realParser(10), rationalParser(10)
{
	parserThread = _parserThread;
}

ParserThread::SolvingThread::~SolvingThread()
{
}

void ParserThread::SolvingThread::operator()()
{
	boost::mutex mut;

	parsers.push_back((BigNumbersParser::Parser<Real>*)new BigNumbersParser::Parser<Real>(10));
	parsers.push_back((BigNumbersParser::Parser<Integer>*)new BigNumbersParser::Parser<Integer>(10));
	parsers.push_back((BigNumbersParser::Parser<Rational>*)new BigNumbersParser::Parser<Rational>(10));
	
	while (!parserThread->exit)
	{
		boost::unique_lock<boost::mutex> lock(mut);
		parserThread->expressionsReady.wait(lock);

		//get the expressions
		parserThread->expressionsMutex.lock();
		expressions = parserThread->expressionsToSolve;
		parserThread->expressionsToSolve.clear();
		parserThread->expressionsMutex.unlock();
		
		//solve the expressions
		for (int i = 0, j; i < (int)expressions.size(); ++i)
		{
			ParserExpression expr = expressions[i];
			ParserException firstException;
			
			//try solve the expression with the given parsers in their order
			for (j = 0; j < (int)parsers.size(); ++j)
			{
				try
				{
					try
					{
						BigNumbersParser::Parser<Integer>* p = boost::get<BigNumbersParser::Parser<Integer>*>(parsers[j]);
						Integer res = p->Parse(expr.expression, expr.precision);
						expr.result = res;
						break;
					}
					catch (boost::bad_get)
					{
					}
					
					try
					{
						BigNumbersParser::Parser<Real>* p = boost::get<BigNumbersParser::Parser<Real>*>(parsers[j]);
						Real res = p->Parse(expr.expression, expr.precision);
						expr.result = res;
						break;
					}
					catch (boost::bad_get)
					{
					}

					try
					{
						BigNumbersParser::Parser<Rational>* p = boost::get<BigNumbersParser::Parser<Rational>*>(parsers[j]);
						Rational res = p->Parse(expr.expression, expr.precision);
						expr.result = res;
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
				expr.exception = firstException;

			//push the solved result
			parserThread->expressionsMutex.lock();
			parserThread->solvedExpressions.push_back(expr);
			parserThread->expressionsMutex.unlock();
			
			//say what the expression was solved
			//QCoreApplication::postEvent(parserThread->wnd, &parserThread->updateEvent);
			QCoreApplication::postEvent(parserThread->wnd, new QEvent((QEvent::Type)FormulaWnd::updateEventId));
		}
	}

	for (int i = 0; i < (int)parsers.size(); ++i)
	{
		try
		{
			BigNumbersParser::Parser<Integer>* p = boost::get<BigNumbersParser::Parser<Integer>*>(parsers[i]);
			delete p;
			continue;
		}
		catch (boost::bad_get)
		{
		}

		try
		{
			BigNumbersParser::Parser<Real>* p = boost::get<BigNumbersParser::Parser<Real>*>(parsers[i]);
			delete p;
			continue;
		}
		catch (boost::bad_get)
		{
		}

		try
		{
			BigNumbersParser::Parser<Rational>* p = boost::get<BigNumbersParser::Parser<Rational>*>(parsers[i]);
			delete p;
			continue;
		}
		catch (boost::bad_get)
		{
		}
	}
}
