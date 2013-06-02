#include "EquationFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"
#include <QFontMetrics>
#include <QMenu>
#include "../Main/FormulaWnd.h"

/**
 * Default constructor.
 */
EquationFormulaNode::EquationFormulaNode()
{
	type = EQUATION_NODE;
}

/**
 * Constructor.
 * @param [in,out] _parent The parent node.
 * @param [in,out] wnd The formula window.
 */
EquationFormulaNode::EquationFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : CompoundFormulaNode(_parent, wnd)
{
	shape = AddShapeNode();
	resNode = NULL;
	type = EQUATION_NODE;

#ifdef _DEBUG
	name = "EquationFormulaNode";
#endif
}

/**
 * Destructor.
 */
EquationFormulaNode::~EquationFormulaNode()
{
}

/**
 * Remakes this node.
 */
void EquationFormulaNode::Remake()
{
	childNodes->Remake();
	
	shape->ClearShapes();
	
	if (childNodes->Count() > 0)
	{
		if (!resNode)
		{
			//make a result node
			resNode = new ResultFormulaNode(this, wnd);
			AddChild(resNode);
			
			//resNode->AddAutoResultNode(wnd->settings->value("auto/precision", 8).toInt(), wnd->settings->value("auto/exp", 3).toInt());
			resNode->AddAutoResultNode(wnd->settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt(), 
				wnd->settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt(), 
				(ExpressionNotation)wnd->settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt(), 
				(FractionType)wnd->settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt());
		}
		
		FormulaNode* left = (*this)[0];
		ParserString expr;
		//parse the left nodes
		left->Parse(expr);
		resNode->SetExpression(expr);

		//place the child nodes
		int cx = left->GetBoundingRect().width();
		int cy = max(left->GetBaseline(), resNode->GetBaseline());
		baseline = cy;
		
		QFont font = settings->GetTextFormulaNodeFont(level);
		QFontMetrics m(font);
		QRect r = m.boundingRect("=");

		int w = r.width();
		int h = r.height();

		left->Move(0, baseline - left->GetBaseline());
		//draw the shape
		shape->AddFillRect(0, h * 0.4, w * 0.8, h < 20 ? 1 : h * 0.05, QColor("black"));
		shape->AddFillRect(0, h >= 20 ? h * 0.55 : h * 0.5 + 2, w * 0.8, h < 20 ? 1 : h * 0.05, QColor("black"));
		shape->Move(cx + w * 0.1 + settings->GetValue("InterNodeSpace", level), cy - font.pointSize());
		resNode->Move(cx + w * 1.1 + settings->GetValue("InterNodeSpace", level) * 2, baseline - resNode->GetBaseline());

		//the shape, that wides the node's bounds for getting mouse events
		r = m.boundingRect("T");
		shape->AddFillRect(0, 0, r.width(), r.height(), QColor("white"), 0);

		shape->GetBoundingRect().setCoords(0, 0, w, h);

		UpdateBoundingRect();
	}
}

/**
 * Updates the bounding rectangle.
 */
void EquationFormulaNode::UpdateBoundingRect()
{
	CompoundFormulaNode::UpdateBoundingRect();

	QFont font = settings->GetTextFormulaNodeFont(level);
	QFontMetrics m(font);
	QRect r = m.boundingRect("=");
	shape->GetBoundingRect().setWidth(r.width());
	shape->GetBoundingRect().setHeight(r.height());
}

/**
 * Makes a deep copy of this object.
 * @param [in] p The parent node.
 * @return A copy of this object.
 */
FormulaNode* EquationFormulaNode::Clone(FormulaNode* p)
{
	return new EquationFormulaNode(parent, wnd);
}

void EquationFormulaNode::Parse(ParserString& expr)
{
	if (childNodes->Count() > 0)
		(*childNodes)[0]->Parse(expr);
}

/**
 * Executes the insert text operation.
 * @param [in] nodeEvent The node event.
 * @return false because there is no possibility of inserting a text into this node
 */
bool EquationFormulaNode::DoInsertText(Command* command)
{
	return false;
}

/**
 * Makes a context menu.
 * @param [in] menu The parent menu.
 */
void EquationFormulaNode::MakeContextMenu(QMenu* menu)
{
	QMenu* subMenu = new QMenu("Add result", menu);

	menu->addMenu(subMenu);
	
	QAction* a = new QAction(tr("Auto"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnAddAutoResult()));

	a = new QAction(tr("Scientific"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnAddRealResult()));

	a = new QAction(tr("Integer"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnAddIntegerResult()));

	a = new QAction(tr("Rational"), subMenu);
	subMenu->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(OnAddRationalResult()));
}

/**
 * Executes the add automatic result node.
 */
void EquationFormulaNode::OnAddAutoResult()
{
	resNode->AddAutoResultNode(wnd->settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt(), 
		wnd->settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt(), 
		(ExpressionNotation)wnd->settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt(), 
		(FractionType)wnd->settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt());
	Remake();
}

/**
 * Executes the add real result node.
 */
void EquationFormulaNode::OnAddRealResult()
{
	resNode->AddRealResultNode(wnd->settings->value("auto/precision", 8).toInt(), wnd->settings->value("auto/exp", 3).toInt());
	Remake();
}

/**
 * Executes the add integer result node.
 */
void EquationFormulaNode::OnAddIntegerResult()
{
	resNode->AddIntegerResultNode(DECIMAL_NOTATION);
	Remake();
}

/**
 * Executes the add rational result node.
 */
void EquationFormulaNode::OnAddRationalResult()
{
	resNode->AddRationalResultNode(IMPROPER_FRACTION);
	Remake();
}
