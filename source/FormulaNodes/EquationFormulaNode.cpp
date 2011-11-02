#include "EquationFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "ShapeFormulaNode.h"
#include <QFontMetrics>
#include "../Main/FormulaWnd.h"

/**
 * Default constructor.
 */
EquationFormulaNode::EquationFormulaNode()
{
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
			
			AutoParserExpression expr1(this, wnd->settings->value("auto/precision", 8).toInt(), wnd->settings->value("auto/exp", 3).toInt());
			resNode->AddResultNode(ParserExpressionVariant(expr1));
			
			RealParserExpression expr2(this, wnd->settings->value("auto/precision", 8).toInt(), wnd->settings->value("auto/exp", 3).toInt());
			resNode->AddResultNode(ParserExpressionVariant(expr2));

			IntegerParserExpression expr3(this, DECIMAL_NOTATION);
			resNode->AddResultNode(ParserExpressionVariant(expr3));

			RationalParserExpression expr4(this, IMPROPER_FRACTION);
			resNode->AddResultNode(ParserExpressionVariant(expr4));
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
 * @return A copy of this object.
 */
FormulaNode* EquationFormulaNode::Clone()
{
	return new EquationFormulaNode(parent, wnd);
}
