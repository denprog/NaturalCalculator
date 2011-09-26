#include "DocumentFormulaNode.h"
#include "FormulaNodesCollection.h"

DocumentFormulaNode::DocumentFormulaNode()
{
}

DocumentFormulaNode::DocumentFormulaNode(FormulaWnd* wnd) : FormulaNode(NULL, wnd), scene(wnd->scene)
{
}

DocumentFormulaNode::~DocumentFormulaNode()
{
}

void DocumentFormulaNode::Remake()
{
	FormulaNode::Remake();
	
	int y = 0;
	
	for (int i = 0; i < childNodes->Count(); ++i)
	{
		FormulaNode* n = (*this)[i];
		n->Move(0, y);
		y += n->boundingRect.height();
	}
	
	UpdateBoundingRect();
	boundingRect.setWidth(boundingRect.width() + 2);
	boundingRect.setHeight(boundingRect.height() + 2);
}

FormulaNode* DocumentFormulaNode::Clone()
{
	return NULL;
}

void DocumentFormulaNode::AddLine()
{
	RootFormulaNode* rootNode = new RootFormulaNode(this);
	EmptyFormulaNode* n = new EmptyFormulaNode(rootNode);
	rootNode->AddChild(n);
	AddChild(rootNode);
}

SharedCaretState DocumentFormulaNode::GetFirstPosition()
{
	return childNodes->GetFirst()->GetFirstPosition();
}

SharedCaretState DocumentFormulaNode::GetLastPosition()
{
	return childNodes->GetLast()->GetLastPosition();
}

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

bool DocumentFormulaNode::UndoInsertLine(NodeEvent& nodeEvent)
{
	SharedCaretState c = any_cast<SharedCaretState>(nodeEvent["caretState"]);
	FormulaNode* node = c->GetNode();
	int pos = GetFirstLevelChildPos(node);
	RemoveChild(pos);
	
	return true;
}
