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
	void RemoveExpression(int id);
	bool GetSolvedExpression(int id, ParserExpression& expr);
		
private:
	class SolvingThread //: public boost::static_visitor<void>
	{
	public:
		SolvingThread(ParserThread* _parserThread);
		~SolvingThread();
		
	public:
		void operator()();
		
		//Real operator()(Real const&);
		//Integer operator()(Integer const&);
		//Rational operator()(Rational const&);

	//private:
	//	template<typename Number>
	//	struct Parser
	//	{
	//		Number Parse(string& expression, int precision)
	//		{
	//			return parser<Number>.Parse(expression, precision);
	//		}
	//		
	//		BigNumbersParser::Parser<Number> parser;
	//	};
		
	private:		
		ParserThread* parserThread;
		vector<ParserExpression> expressions;
		//ParserExpression currentExpression;
		
		//parsers
		//BigNumbersParser::Parser<Integer> integerParser;
		//BigNumbersParser::Parser<Real> realParser;
		//BigNumbersParser::Parser<Rational> rationalParser;
		
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
	QEvent updateEvent; //event for updating the main window
	boost::condition_variable expressionsReady;
	bool exit; //exit flag for the thread
};

#endif
