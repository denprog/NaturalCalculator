#ifndef PARSERTHREAD_H
#define PARSERTHREAD_H

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <vector>
#include <QEvent>
#include "ParserExpression.h"
#include "BigNumbersParser.h"

using namespace std;
using namespace BigNumbersParser;

class FormulaWnd;

class ParserThread
{
public:
	ParserThread(FormulaWnd* _wnd);
	~ParserThread();

public:
	void AddExpression(ParserExpression& expr);
	void RemoveExpression(HierarchyPos& id);
	bool GetSolvedExpression(HierarchyPos& pos, ParserExpression& expr);
		
private:
	class SolvingThread
	{
	public:
		SolvingThread(ParserThread* _parserThread);
		~SolvingThread();
		
	public:
		void operator()();
		
	private:		
		ParserThread* parserThread;
		vector<ParserExpression> expressions;
		
		//vector of parsers with their difined order of solving
		vector<boost::variant<
			BigNumbersParser::Parser<Integer>*, 
			BigNumbersParser::Parser<Real>*, 
			BigNumbersParser::Parser<Rational>* 
			> > parsers;
	};
	
private:
	boost::thread* thread;
	FormulaWnd* wnd;
	vector<ParserExpression> expressionsToSolve; //the thread gets expressions from here
	vector<ParserExpression> solvedExpressions; //the thread puts solved expressions here
	boost::mutex expressionsMutex; //locks these vectors for writing and reading
	boost::condition_variable expressionsReady;
	bool exit; //exit flag for the thread
};

#endif
