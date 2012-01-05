#include "DocumentFormulaNode.h"
#include "FormulaNodesCollection.h"

/**
 * Default constructor.
 */
DocumentFormulaNode::DocumentFormulaNode()
{
}

/**
 * Constructor.
 * @param [in] wnd The formula window.
 */
DocumentFormulaNode::DocumentFormulaNode(FormulaWnd* wnd) : FormulaNode(NULL, wnd), scene(wnd->scene)
{
}

/**
 * Destructor.
 */
DocumentFormulaNode::~DocumentFormulaNode()
{
}

/**
 * Remakes this node.
 */
void DocumentFormulaNode::Remake()
{
	FormulaNode::Remake();
	
	int y = 0;
	
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->Move(0, y);
		y += n->GetBoundingRect().height();
	}
	
	UpdateBoundingRect();
	boundingRect.setWidth(boundingRect.width() + 2);
	boundingRect.setHeight(boundingRect.height() + 2);
}

/**
 * The document can not be copied.
 * @param [in] p The parent node.
 * @return null.
 */
FormulaNode* DocumentFormulaNode::Clone(FormulaNode* p)
{
	return NULL;
}

/**
 * Adds a formula line.
 */
void DocumentFormulaNode::AddLine()
{
	RootFormulaNode* rootNode = new RootFormulaNode(this);
	EmptyFormulaNode* n = new EmptyFormulaNode(rootNode);
	rootNode->AddChild(n);
	AddChild(rootNode);
}

/**
 * Clears this node to its blank state.
 */
void DocumentFormulaNode::Clear()
{
	childNodes->Clear();
	AddLine();
}

/**
 * Gets the first caret position.
 * @return The first caret position.
 */
SharedCaretState DocumentFormulaNode::GetFirstPosition()
{
	return childNodes->GetFirst()->GetFirstPosition();
}

/**
 * Gets the last caret position.
 * @return The last caret position.
 */
SharedCaretState DocumentFormulaNode::GetLastPosition()
{
	return childNodes->GetLast()->GetLastPosition();
}

/**
 * Gets the next caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The next caret position.
 */
SharedCaretState DocumentFormulaNode::GetNextPosition(SharedCaretState& relativeState)
{
	SharedCaretState res;
	
	if (relativeState)
	{
		FormulaNode* n = relativeState->GetNode();
		int pos = GetFirstLevelChildPos(n);
		if (pos < childNodes->Count() - 1)
			res = (*this)[pos + 1]->GetFirstPosition();
	}
	
	return res;
}

/**
 * Gets the previous caret position.
 * @param [in,out] relativeState Relative caret state.
 * @return The previous caret position.
 */
SharedCaretState DocumentFormulaNode::GetPreviousPosition(SharedCaretState& relativeState)
{
	SharedCaretState res;

	if (relativeState)
	{
		FormulaNode* n = relativeState->GetNode();
		int pos = GetFirstLevelChildPos(n);
		if (pos > 0)
			res = (*this)[pos - 1]->GetLastPosition();
	}
	
	return res;
}

/**
 * Executes the insert line operation.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool DocumentFormulaNode::DoInsertLine(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* node = c->GetNode();
	int pos = GetFirstLevelChildPos(node);
	RootFormulaNode* rootNode = new RootFormulaNode(this);
	EmptyFormulaNode* n = new EmptyFormulaNode(rootNode);
	rootNode->AddChild(n);
	InsertChild(rootNode, pos + 1);
	
	nodeEvent["undoAction"] = CommandAction(rootNode, 0, &FormulaNode::UndoInsertLine);
	c->SetToNode(rootNode, 0);

	return true;
}

/**
 * Undo insert line.
 * @param [in,out] nodeEvent The node event.
 * @return true if it succeeds, false if it fails.
 */
bool DocumentFormulaNode::UndoInsertLine(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* node = c->GetNode();
	int pos = GetFirstLevelChildPos(node);
	RemoveChild(pos);
	
	return true;
}
