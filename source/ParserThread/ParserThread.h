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
	void AddExpression(ParserExpressionVariant& expr);
	void RemoveExpression(ParserExpressionVariant& expr);
	bool GetSolvedExpression(ParserExpressionVariant& expr);
		
private:
	class SolvingThread : public boost::static_visitor<void>
	{
	public:
		SolvingThread(ParserThread* _parserThread);
		~SolvingThread();
		
	public:
		void operator()();
	
		void operator()(RealParserExpression const& expr) const;
		void operator()(IntegerParserExpression const& expr) const;
		void operator()(RationalParserExpression const& expr) const;
		void operator()(AutoParserExpression const& expr) const;
		
	private:		
		ParserThread* parserThread;
		vector<ParserExpressionVariant> expressions;
		
		//vector of parsers with their difined order of solving an auto result node
		vector<boost::variant<
			BigNumbersParser::Parser<Integer>*, 
			BigNumbersParser::Parser<Real>*, 
			BigNumbersParser::Parser<Rational>* 
			> > parsers;
		
		Parser<Real>* realParser;
		Parser<Integer>* integerParser;
		Parser<Rational>* rationalParser;
	};
	
private:
	boost::thread* thread;
	FormulaWnd* wnd;
	
	vector<ParserExpressionVariant> expressionsToSolve; //the thread gets expressions from here
	vector<ParserExpressionVariant> solvedExpressions; //the thread puts solved expressions here
	typedef vector<ParserExpressionVariant>::iterator ParserExpressionVariantIter;
	
	boost::mutex expressionsMutex; //locks these vectors for writing and reading
	boost::condition_variable expressionsReady;
	bool exit; //exit flag for the thread
};

#endif
