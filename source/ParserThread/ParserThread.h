#ifndef PARSERTHREAD_H
#define PARSERTHREAD_H

#include <QObject>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <vector>
#include <QEvent>
#include "ParserExpression.h"
#include "BigNumbersParser.h"

using namespace BigNumbersParser;

class FormulaWnd;
class Settings;

/**
 * Parser thread.
 */
class ParserThread
{
public:
	ParserThread(FormulaWnd* _wnd);
	~ParserThread();

public:
	friend class SolvingThread;
	
public:
	void AddExpression(ParserExpressionVariant& expr);
	void RemoveExpression(ParserExpressionVariant& expr);
	bool GetSolvedExpression(ParserExpressionVariant& expr);

private:
	boost::thread* thread;
	FormulaWnd* wnd;
	
	std::vector<ParserExpressionVariant> expressionsToSolve; //the thread gets expressions from here
	std::vector<ParserExpressionVariant> solvedExpressions; //the thread puts solved expressions here
	typedef vector<ParserExpressionVariant>::iterator ParserExpressionVariantIter;
	
	boost::mutex expressionsMutex; //locks these vectors for writing and reading
	boost::condition_variable expressionsReady;
	boost::mutex parserMutex;
	bool exit; //exit flag for the thread
};

/**
 * Solving thread.
 */
class SolvingThread : public QObject, public boost::static_visitor<void>
{
	Q_OBJECT

public:
	SolvingThread(ParserThread* _parserThread, Settings* _settings);
	SolvingThread(const SolvingThread& source);
	~SolvingThread();
	
public:
	void operator()();

	void operator()(RealParserExpression const& expr) const;
	void operator()(IntegerParserExpression const& expr) const;
	void operator()(RationalParserExpression const& expr) const;
	void operator()(AutoParserExpression const& expr) const;

private:
	void UpdateParsers();

public slots:
	void OnSettingsChanged(const QString& prefix, const QString& key);
			
private:		
	ParserThread* parserThread;
	vector<ParserExpressionVariant> expressions;
	Settings* settings;
	
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

#endif
