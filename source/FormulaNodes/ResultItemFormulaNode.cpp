#include "ResultItemFormulaNode.h"
#include "../Main/FormulaWnd.h"
#include <QMenu>

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The formula window.
 */
ResultItemFormulaNode::ResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : GroupFormulaNode(_parent, wnd)
{
	type = RESULT_ITEM_NODE;
}

/**
 * Executes the action of presentation as automatic result node.
 */
void ResultItemFormulaNode::OnPresentAsAutoResult()
{
	((ResultFormulaNode*)parent)->SetAutoResult(settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt(), 
		settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt(), 
		(ExpressionNotation)settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt(), 
		(FractionType)settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt());
}

/**
 * Executes the action of presentation as real result node.
 */
void ResultItemFormulaNode::OnPresentAsRealResult()
{
	((ResultFormulaNode*)parent)->SetRealResult(settings->value("auto/precision", 8).toInt(), settings->value("auto/exp", 3).toInt());
}

/**
 * Executes the action of presentation as integer result node.
 */
void ResultItemFormulaNode::OnPresentAsIntegerResult()
{
	((ResultFormulaNode*)parent)->SetIntegerResult(DECIMAL_NOTATION);
}

/**
 * Executes the action of presentation as rational result node.
 */
void ResultItemFormulaNode::OnPresentAsRationalResult()
{
	((ResultFormulaNode*)parent)->SetRationalResult(IMPROPER_FRACTION);
}

/**
 * Constructor.
 * @param [in] _parent The parent formula node.
 * @param [in] wnd The formula window.
 * @param _precision The precision.
 * @param _exp The exponent.
 */
AutoResultItemFormulaNode::AutoResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, int _realPrecision, int _realExp, 
	ExpressionNotation _notation, FractionType _fractionType) : 
	ResultItemFormulaNode(_parent, wnd), realPrecision(_realPrecision), realExp(_realExp), notation(_notation), fractionType(_fractionType)
{
	Normalize();
}

/**
 * Updates the expression.
 */
void AutoResultItemFormulaNode::Normalize()
{
	expression = AutoParserExpression(this, realPrecision, realExp, notation, fractionType);
}

/**
 * Makes a context menu.
 * @param [in] menu The context menu.
 */
void AutoResultItemFormulaNode::MakeContextMenu(QMenu* menu)
{
	if (menu->actions().count() > 0)
		menu->addSeparator();

	//create the "present as" sub menu
	QMenu* subMenu = new QMenu("Present as", menu);
	menu->addMenu(subMenu);
	
	QAction* a = new QAction(tr("Auto"), subMenu);
	a->setCheckable(true);
	a->setChecked(true);
	subMenu->addAction(a);
	
	a = new QAction(tr("Scientific"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRealResult()));

	a = new QAction(tr("Integer"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsIntegerResult()));

	a = new QAction(tr("Rational"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRationalResult()));
}

#ifdef TEST
std::string AutoResultItemFormulaNode::ParseStructure()
{
	return "$auto(" + ResultItemFormulaNode::ParseStructure() + ")";
}
#endif

bool AutoResultItemFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent)
{
	std::string::iterator i = begin;
	
	if (FindSubstring(begin, end, "$auto"))
	{
		int realPrecision = _parent->wnd->settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt();
		int realExp = _parent->wnd->settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt();
		ExpressionNotation notation = (ExpressionNotation)_parent->wnd->settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt();
		FractionType fractionType = (FractionType)_parent->wnd->settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt();
		
		i += 5;
		std::vector<int> params;
		if (FormulaNode::GetIntParams(i, end, params) && params.size() == 4)
		{
			realPrecision = params[0];
			realExp = params[1];
			notation = (ExpressionNotation)params[2];
			fractionType = (FractionType)params[3];
		}
		
		_parent->AddChild(new AutoResultItemFormulaNode(_parent, _parent->wnd, realPrecision, realExp, notation, fractionType));
		begin = i;
		return true;
	}
	
	return false;
}

std::string AutoResultItemFormulaNode::ToString()
{
	std::stringstream res;
	res << "$auto(";
	res << realPrecision << ",";
	res << realExp << ",";
	res << (int)notation << ",";
	res << (int)fractionType << ")";
	return res.str();
}

//RealResultItemFormulaNode

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The formula window.
 * @param _precision The precision.
 * @param _exp The exponent.
 */
RealResultItemFormulaNode::RealResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, int _precision, int _exp) : 
	ResultItemFormulaNode(_parent, wnd), precision(_precision), exp(_exp)
{
	Normalize();
}

/**
 * Updates the expression.
 */
void RealResultItemFormulaNode::Normalize()
{
	expression = RealParserExpression(this, precision, exp);
}

/**
 * Makes a context menu.
 * @param [in] menu The parent menu.
 */
void RealResultItemFormulaNode::MakeContextMenu(QMenu* menu)
{
	if (menu->actions().count() > 0)
		menu->addSeparator();

	//create the "present as" sub menu
	QMenu* subMenu = new QMenu("Present as", menu);
	menu->addMenu(subMenu);
	
	QAction* a = new QAction(tr("Auto"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsAutoResult()));

	a = new QAction(tr("Scientific"), subMenu);
	a->setCheckable(true);
	a->setChecked(true);
	subMenu->addAction(a);
	
	a = new QAction(tr("Integer"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsIntegerResult()));

	a = new QAction(tr("Rational"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRationalResult()));
}

#ifdef TEST
std::string RealResultItemFormulaNode::ParseStructure()
{
	return "$real(" + ResultItemFormulaNode::ParseStructure() + ")";
}
#endif

bool RealResultItemFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent)
{
	std::string::iterator i = begin;
	
	if (FindSubstring(begin, end, "$real"))
	{
		int precision = _parent->wnd->settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt();
		int exp = _parent->wnd->settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt();
		
		i += 5;
		std::vector<int> params;
		if (FormulaNode::GetIntParams(i, end, params) && params.size() == 2)
		{
			precision = params[0];
			exp = params[1];
		}
		
		_parent->AddChild(new RealResultItemFormulaNode(_parent, _parent->wnd, precision, exp));
		begin = i;
		return true;
	}
	
	return false;
}

std::string RealResultItemFormulaNode::ToString()
{
	std::stringstream res;
	res << "$real(";
	res << precision << ")";
	return res.str();
}

//IntegerResultItemFormulaNode

IntegerResultItemFormulaNode::IntegerResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, ExpressionNotation _notation) : 
	ResultItemFormulaNode(_parent, wnd), notation(_notation)
{
	Normalize();
}

/**
 * Updates the expression.
 */
void IntegerResultItemFormulaNode::Normalize()
{
	expression = IntegerParserExpression(this, notation);
}

/**
 * Makes a context menu.
 * @param [in] menu The parent menu.
 */
void IntegerResultItemFormulaNode::MakeContextMenu(QMenu* menu)
{
	if (menu->actions().count() > 0)
		menu->addSeparator();

	//create the "present as" sub menu
	QMenu* subMenu = new QMenu("Present as", menu);
	menu->addMenu(subMenu);
	
	QAction* a = new QAction(tr("Auto"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsAutoResult()));

	a = new QAction(tr("Scientific"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRealResult()));

	a = new QAction(tr("Integer"), subMenu);
	a->setCheckable(true);
	a->setChecked(true);
	subMenu->addAction(a);
	
	a = new QAction(tr("Rational"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRationalResult()));
}

#ifdef TEST
std::string IntegerResultItemFormulaNode::ParseStructure()
{
	return "$integer(" + ResultItemFormulaNode::ParseStructure() + ")";
}
#endif

bool IntegerResultItemFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent)
{
	std::string::iterator i = begin;
	
	if (FindSubstring(begin, end, "$integer"))
	{
		ExpressionNotation notation = (ExpressionNotation)_parent->wnd->settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt();
		
		i += 8;
		std::vector<int> params;
		if (FormulaNode::GetIntParams(i, end, params) && params.size() == 1)
		{
			notation = (ExpressionNotation)params[0];
		}
		
		_parent->AddChild(new IntegerResultItemFormulaNode(_parent, _parent->wnd, notation));
		begin = i;
		return true;
	}
	
	return false;
}

std::string IntegerResultItemFormulaNode::ToString()
{
	std::stringstream res;
	res << "$integer(";
	res << (int)notation << ")";
	return res.str();
}

//RationalResultItemFormulaNode

/**
 * Constructor.
 * @param [in] _parent The parent node.
 * @param [in] wnd The formula window.
 * @param _fractionType	Type of the fraction.
 */
RationalResultItemFormulaNode::RationalResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, FractionType _fractionType) : 
	ResultItemFormulaNode(_parent, wnd), fractionType(_fractionType)
{
	Normalize();
}

/**
 * Updates the expression.
 */
void RationalResultItemFormulaNode::Normalize()
{
	expression = RationalParserExpression(this, fractionType);
}

/**
 * Makes a context menu.
 * @param [in] menu The parent menu.
 */
void RationalResultItemFormulaNode::MakeContextMenu(QMenu* menu)
{
	if (menu->actions().count() > 0)
		menu->addSeparator();

	//create the "present as" sub menu
	QMenu* subMenu = new QMenu("Present as", menu);
	menu->addMenu(subMenu);
	
	QAction* a = new QAction(tr("Auto"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsAutoResult()));

	a = new QAction(tr("Scientific"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRealResult()));

	a = new QAction(tr("Integer"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsIntegerResult()));

	a = new QAction(tr("Rational"), subMenu);
	a->setCheckable(true);
	a->setChecked(true);
	subMenu->addAction(a);
}

#ifdef TEST
std::string RationalResultItemFormulaNode::ParseStructure()
{
	return "$rational(" + ResultItemFormulaNode::ParseStructure() + ")";
}
#endif

bool RationalResultItemFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* _parent)
{
	std::string::iterator i = begin;
	
	if (FindSubstring(begin, end, "$rational"))
	{
		FractionType fractionType = (FractionType)_parent->wnd->settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt();
		
		i += 9;
		std::vector<int> params;
		if (FormulaNode::GetIntParams(i, end, params) && params.size() == 1)
		{
			fractionType = (FractionType)params[0];
		}
		
		_parent->AddChild(new RationalResultItemFormulaNode(_parent, _parent->wnd, fractionType));
		begin = i;
		return true;
	}
	
	return false;
}

std::string RationalResultItemFormulaNode::ToString()
{
	std::stringstream res;
	res << "$rational(";
	res << (int)fractionType << ")";
	return res.str();
}
