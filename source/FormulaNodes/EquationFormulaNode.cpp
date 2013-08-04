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
	type = EQUATION_NODE;
	
	left = new GroupFormulaNode(this, wnd);
	AddChild(left);
	shape = AddShapeNode();
	right = new GroupFormulaNode(this, wnd);
	AddChild(right);
	result = NULL;
}

/**
 * Destructor.
 */
EquationFormulaNode::~EquationFormulaNode()
{
}

void EquationFormulaNode::RemoveChildNodes()
{
	childNodes->Clear();
	left = NULL;
	right = NULL;
	shape = NULL;
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
		if (right->ChildrenCount() == 0)
		{
			//make a result node
			result = new ResultFormulaNode(this, wnd);
			right->AddChild(result);
			
			result->AddAutoResultNode(wnd->settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt(), 
				wnd->settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt(), 
				(ExpressionNotation)wnd->settings->Load("IntegerNumbers", "notation", DECIMAL_NOTATION).toInt(), 
				(FractionType)wnd->settings->Load("RationalNumbers", "form", PROPER_FRACTION).toInt());
		}
		else
			result = (ResultFormulaNode*)(*right->childNodes)[0];
		
		ParserString expr;
		//parse the left nodes
		left->Parse(expr);
		result->SetExpression(expr);

		//place the child nodes
		int cx = left->boundingRect.width();
		int cy = max(left->baseline, result->baseline);
		baseline = cy;
		
		QFont font = settings->GetTextFormulaNodeFont(level);
		QFontMetrics m(font);
		QRect r = m.boundingRect("=");

		int w = r.width();
		int h = r.height();

		left->Move(0, baseline - left->baseline);
		//draw the shape
		shape->AddFillRect(0, h * 0.4, w * 0.8, h < 20 ? 1 : h * 0.05, QColor("black"));
		shape->AddFillRect(0, h >= 20 ? h * 0.55 : h * 0.5 + 2, w * 0.8, h < 20 ? 1 : h * 0.05, QColor("black"));
		shape->Move(cx + w * 0.1 + settings->GetValue("InterNodeSpace", level), cy - font.pointSize());
		result->Move(cx + w * 1.1 + settings->GetValue("InterNodeSpace", level) * 2, baseline - result->baseline);

		//the shape, that wides the node's bounds for getting mouse events
		r = m.boundingRect("T");
		shape->AddFillRect(0, 0, r.width(), r.height(), QColor("white"), 0);

		shape->boundingRect.setCoords(0, 0, w, h);

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
	shape->boundingRect.setWidth(r.width());
	shape->boundingRect.setHeight(r.height());
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

#ifdef TEST
std::string EquationFormulaNode::ParseStructure()
{
	return left->ParseStructure() + "=" + result->ParseStructure();
}
#endif

bool EquationFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	if (*begin == '=')
	{
		EquationFormulaNode* e = new EquationFormulaNode(parent, parent->wnd);
		if (parent->ChildrenCount() > 0)
		{
			if (parent->type == GROUP_NODE)
				e->left->MoveChild((*parent->childNodes)[parent->ChildrenCount() - 1], 0);
			else
				e->left->MoveChild((*parent)[0], 0);
		}
		else
			e->left->AddChild(new EmptyFormulaNode(e->left));

		++begin;
		FormulaNode::FromString(begin, end, e->right);
		e->result = (ResultFormulaNode*)(*e->right->childNodes)[0];
		parent->AddChild(e);
		return true;
	}
	
	return false;
}

std::string EquationFormulaNode::ToString()
{
	return left->ToString() + "=" + right->ToString();
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
	assert(result);
	result->AddAutoResultNode(wnd->settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt(), 
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
	assert(result);
	result->AddRealResultNode(wnd->settings->value("auto/precision", 8).toInt(), wnd->settings->value("auto/exp", 3).toInt());
	Remake();
}

/**
 * Executes the add integer result node.
 */
void EquationFormulaNode::OnAddIntegerResult()
{
	assert(result);
	result->AddIntegerResultNode(DECIMAL_NOTATION);
	Remake();
}

/**
 * Executes the add rational result node.
 */
void EquationFormulaNode::OnAddRationalResult()
{
	assert(result);
	result->AddRationalResultNode(IMPROPER_FRACTION);
	Remake();
}
