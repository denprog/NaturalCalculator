#include "ParserThread.h"
#include "../Main/FormulaWnd.h"
#include <QCoreApplication>

ParserThread::ParserThread(FormulaWnd* _wnd) : wnd(_wnd), updateEvent((QEvent::Type)FormulaWnd::updateEventId)
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

void ParserThread::RemoveExpression(int id)
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

bool ParserThread::GetSolvedExpression(int id, ParserExpression& expr)
{
	boost::mutex::scoped_lock lock(expressionsMutex);

	vector<ParserExpression>::iterator it = find(solvedExpressions.begin(), solvedExpressions.end(), id);
	if (it != solvedExpressions.end())
	{
		expr = *it;
		return true;
	}
	
	return false;
}

ParserThread::SolvingThread::SolvingThread(ParserThread* _parserThread) //: integerParser(10), realParser(10), rationalParser(10)
{
	parserThread = _parserThread;
	parsers.push_back(new BigNumbersParser::Parser<Integer>(10));
	parsers.push_back(new BigNumbersParser::Parser<Real>(10));
	parsers.push_back(new BigNumbersParser::Parser<Rational>(10));
}

ParserThread::SolvingThread::~SolvingThread()
{
	for (int i = 0; i < (int)parsers.size(); ++i)
	{
		try
		{
			BigNumbersParser::Parser<Integer>* p = any_cast<BigNumbersParser::Parser<Integer>* >(parsers[i]);
			delete p;
			continue;
		}
		catch (bad_cast)
		{
		}

		try
		{
			BigNumbersParser::Parser<Real>* p = any_cast<BigNumbersParser::Parser<Real>* >(parsers[i]);
			delete p;
			continue;
		}
		catch (bad_cast)
		{
		}

		try
		{
			BigNumbersParser::Parser<Rational>* p = any_cast<BigNumbersParser::Parser<Rational>* >(parsers[i]);
			delete p;
			continue;
		}
		catch (bad_cast)
		{
		}
	}
}

void ParserThread::SolvingThread::operator()()
{
	boost::mutex threadMutex;
	
	while (!parserThread->exit)
	{
		boost::unique_lock<boost::mutex> lock(threadMutex);
		parserThread->expressionsReady.wait(lock);

		parserThread->expressionsMutex.lock();
		expressions = parserThread->expressionsToSolve;
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
						BigNumbersParser::Parser<Integer>* p = any_cast<BigNumbersParser::Parser<Integer>* >(parsers[j]);
						Integer res = p->Parse(expr.expression, expr.precision);
						expr.result = res;
						break;
					}
					catch (boost::bad_get)
					{
					}
					
					try
					{
						BigNumbersParser::Parser<Real>* p = any_cast<BigNumbersParser::Parser<Real>* >(parsers[j]);
						Real res = p->Parse(expr.expression, expr.precision);
						expr.result = res;
						break;
					}
					catch (boost::bad_get)
					{
					}

					try
					{
						BigNumbersParser::Parser<Rational>* p = any_cast<BigNumbersParser::Parser<Rational>* >(parsers[j]);
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
			parserThread->expressionsToSolve.push_back(expr);
			parserThread->expressionsMutex.unlock();
			
			//say what the expression was solved
			QCoreApplication::postEvent(parserThread->wnd, &parserThread->updateEvent);
		}
	}
}
//
//Real ParserThread::SolvingThread::operator()(Real const&)
//{
//	return realParser.Parse(currentExpression.expression, currentExpression.precision);
//	//return Real();
//}
//
//Integer ParserThread::SolvingThread::operator()(Integer const&)
//{
//	return Integer();
//}
//
//Rational ParserThread::SolvingThread::operator()(Rational const&)
//{
//	return Rational();
//}
