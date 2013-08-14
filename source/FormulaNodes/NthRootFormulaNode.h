#ifndef NTHROOTFORMULANODE_H
#define	NTHROOTFORMULANODE_H

#include "SquareRootFormulaNode.h"

class NthRootFormulaNode : public SquareRootFormulaNode
{
public:
	NthRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd);
	
public:
	virtual void RemoveChildNodes();

	virtual void Remake();
	virtual void UpdateBoundingRect();

	virtual FormulaNode* GetExpression(int pos) const;
	virtual bool IsShapeVisible(int pos) const;

	virtual FormulaNode* Clone(FormulaNode* p);

	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual std::string ParseStructure();
#endif
	
	static bool FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent);
	virtual std::string ToString();
	
	virtual void RenderCaret(const int pos, const int anchor);
	
	//command functions
	virtual bool DoRemoveItem(Command* command);
	
public:
	GroupFormulaNode* degree;
};

#endif
