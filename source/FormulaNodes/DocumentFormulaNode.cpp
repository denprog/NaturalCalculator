#include "DocumentFormulaNode.h"
#include "FormulaNodesCollection.h"
#include "../Main/FormulaWnd.h"

/**
 * Default constructor.
 */
DocumentFormulaNode::DocumentFormulaNode()
{
	type = DOCUMENT_NODE;
}

/**
 * Constructor.
 * @param [in] wnd The formula window.
 */
DocumentFormulaNode::DocumentFormulaNode(FormulaWnd* wnd) : FormulaNode(NULL, wnd), scene(wnd->scene)
{
	type = DOCUMENT_NODE;
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
		y += n->boundingRect.height();
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
SharedCaretState DocumentFormulaNode::GetNextPosition(SharedCaretState relativeState)
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
SharedCaretState DocumentFormulaNode::GetPreviousPosition(SharedCaretState relativeState)
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
bool DocumentFormulaNode::DoInsertLine(Command* command)
{
	command->SaveNodeState(this);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	FormulaNode* node = c->GetNode();
	int pos = GetFirstLevelChildPos(node);
	RootFormulaNode* rootNode = new RootFormulaNode(this);
	EmptyFormulaNode* n = new EmptyFormulaNode(rootNode);
	rootNode->AddChild(n);
	InsertChild(rootNode, pos + 1);
	
	c->SetToNode(rootNode, 0);
	command->afterCaretState = c;

	return true;
}

#ifdef TEST
std::string DocumentFormulaNode::ParseStructure()
{
	std::string res;
	int i;
	for (i = 0; i < ChildrenCount() - 1; ++i)
		res += (*this)[i]->ParseStructure() + "\n\r";
	res += (*this)[i]->ParseStructure();
	return res;
}
#endif

bool DocumentFormulaNode::FromString(std::string& str, DocumentFormulaNode* node)
{
	size_t pos = 0;
	
	while (pos != std::string::npos)
	{
		size_t i = str.find("\n\r", pos);
		if (i == std::string::npos)
		{
			node->AddChild(RootFormulaNode::FromString(str.substr(pos), node->wnd));
			pos = i;
		}
		else
		{
			node->AddChild(RootFormulaNode::FromString(str.substr(pos, i - pos), node->wnd));
			pos = i + 2;
		}
	}
	
	return true;
}

std::string DocumentFormulaNode::ToString()
{
	std::string res;
	int i;
	for (i = 0; i < ChildrenCount() - 1; ++i)
		res += (*this)[i]->ToString() + "\n\r";
	res += (*this)[i]->ToString();
	return res;
}
