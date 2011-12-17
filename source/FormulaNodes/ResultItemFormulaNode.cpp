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
}

/**
 * Destructor.
 */
ResultItemFormulaNode::~ResultItemFormulaNode()
{
}

/**
 * Executes the action of presentation as automatic result node.
 */
void ResultItemFormulaNode::OnPresentAsAutoResult()
{
	AutoResultItemFormulaNode* node = new AutoResultItemFormulaNode(parent, wnd, 
		settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt(), 
		settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt(), 
		(ExpressionNotation)settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt(), 
		(FractionType)settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt());
	node->UpdateExpression();
	parent->ReplaceChild(node, parent->GetChildPos(this));
}

/**
 * Executes the action of presentation as real result node.
 */
void ResultItemFormulaNode::OnPresentAsRealResult()
{
	RealResultItemFormulaNode* node = new RealResultItemFormulaNode(parent, wnd, settings->value("auto/precision", 8).toInt(), 
		settings->value("auto/exp", 3).toInt());
	node->UpdateExpression();
	parent->ReplaceChild(node, parent->GetChildPos(this));
}

/**
 * Executes the action of presentation as integer result node.
 */
void ResultItemFormulaNode::OnPresentAsIntegerResult()
{
	IntegerResultItemFormulaNode* node = new IntegerResultItemFormulaNode(parent, wnd, DECIMAL_NOTATION);
	node->UpdateExpression();
	parent->ReplaceChild(node, parent->GetChildPos(this));
}

/**
 * Executes the action of presentation as rational result node.
 */
void ResultItemFormulaNode::OnPresentAsRationalResult()
{
	RationalResultItemFormulaNode* node = new RationalResultItemFormulaNode(parent, wnd, IMPROPER_FRACTION);
	node->UpdateExpression();
	parent->ReplaceChild(node, parent->GetChildPos(this));
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
}

/**
 * Destructor.
 */
AutoResultItemFormulaNode::~AutoResultItemFormulaNode()
{
}

/**
 * Updates the expression.
 */
void AutoResultItemFormulaNode::UpdateExpression()
{
	expression = AutoParserExpression(this, realPrecision, realExp);
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
	
	QAction* a = new QAction(tr("Scientific"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRealResult()));

	a = new QAction(tr("Integer"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsIntegerResult()));

	a = new QAction(tr("Rational"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRationalResult()));
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
}

/**
 * Destructor.
 */
RealResultItemFormulaNode::~RealResultItemFormulaNode()
{
}

/**
 * Updates the expression.
 */
void RealResultItemFormulaNode::UpdateExpression()
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

	a = new QAction(tr("Integer"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsIntegerResult()));

	a = new QAction(tr("Rational"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRationalResult()));
}

//IntegerResultItemFormulaNode

IntegerResultItemFormulaNode::IntegerResultItemFormulaNode(FormulaNode* _parent, FormulaWnd* wnd, ExpressionNotation _notation) : 
	ResultItemFormulaNode(_parent, wnd), notation(_notation)
{
}

/**
 * Destructor.
 */
IntegerResultItemFormulaNode::~IntegerResultItemFormulaNode()
{
}

/**
 * Updates the expression.
 */
void IntegerResultItemFormulaNode::UpdateExpression()
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

	a = new QAction(tr("Rational"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnPresentAsRationalResult()));
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
}

/**
 * Destructor.
 */
RationalResultItemFormulaNode::~RationalResultItemFormulaNode()
{
}

/**
 * Updates the expression.
 */
void RationalResultItemFormulaNode::UpdateExpression()
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
}
