#include "NthRootFormulaNode.h"
#include "EmptyFormulaNode.h"
#include "../Main/FormulaWnd.h"

//NthRootFormulaNode

NthRootFormulaNode::NthRootFormulaNode(FormulaNode* _parent, FormulaWnd* wnd) : SquareRootFormulaNode(_parent, wnd)
{
	type = NTH_ROOT_NODE;
	degree = new GroupFormulaNode(parent, wnd);
	InsertChild(degree, 0);
}

void NthRootFormulaNode::RemoveChildNodes()
{
	childNodes->Clear();
	radicand = NULL;
	degree = NULL;
}

void NthRootFormulaNode::Remake()
{
	degree->SetLevel(GetLesserLevel());
	SquareRootFormulaNode::Remake();
	GroupFormulaNode::Remake();

	if (childNodes->Count() > 0)
	{
		qreal cx = radicand->boundingRect.height() * 5 / 11;
		qreal offset = radicand->boundingRect.height() / 5;
		if (shape->boundingRect.height() / 2 < degree->boundingRect.height())
		{
			shape->Move(degree->boundingRect.width() + 1, degree->boundingRect.height() - shape->boundingRect.height() / 2);
			radicand->Move(cx * 1.3 + degree->boundingRect.width() + 1, shape->boundingRect.top() + offset);
			baseline = radicand->baseline - shape->boundingRect.height() / 2 + degree->boundingRect.height() + offset;
		}
		else
		{
			degree->Move(0, shape->boundingRect.height() / 2 - degree->boundingRect.height());
			shape->Move(degree->boundingRect.width() + 1, 0);
			radicand->Move(cx * 1.3 + degree->boundingRect.width() + 1, shape->boundingRect.top() + offset);
			baseline = radicand->baseline + offset;
		}
	}
}

void NthRootFormulaNode::UpdateBoundingRect()
{
	CompoundFormulaNode::UpdateBoundingRect();
	
	if (shape->boundingRect.height() / 2 > degree->boundingRect.height())
	{
		boundingRect.setCoords(degree->boundingRect.left(), shape->boundingRect.top(), shape->boundingRect.right(), 
			shape->boundingRect.bottom());
	}
	else
	{
		boundingRect.setCoords(degree->boundingRect.left(), degree->boundingRect.top(), shape->boundingRect.right(), 
			shape->boundingRect.bottom());
	}
	qreal cx = GetExpression(0)->boundingRect.height() * 5 / 11;
	boundingRect.setRight(boundingRect.right() + cx * 0.075);
	boundingRect.moveTo(item->pos().x(), item->pos().y());
}

FormulaNode* NthRootFormulaNode::GetExpression(int pos) const
{
	assert(pos <= 1);
	return (pos == 0 ? degree : radicand);
}

bool NthRootFormulaNode::IsShapeVisible(int pos) const
{
	assert(pos == 0);
	return true;
}

FormulaNode* NthRootFormulaNode::Clone(FormulaNode* p)
{
	NthRootFormulaNode* res = new NthRootFormulaNode(p, wnd);
	res->InsertChild(GetExpression(0)->Clone(res), 0);
	res->InsertChild(GetExpression(1)->Clone(res), 2);
	return res;
}

void NthRootFormulaNode::Parse(ParserString& expr)
{
	expr.Add(std::string("root("), this);
	degree->Parse(expr);
	expr.Add(std::string(","), this);
	radicand->Parse(expr);
	expr.Add(std::string(")"), this);
}

#ifdef TEST
std::string NthRootFormulaNode::ParseStructure()
{
	return "root(" + degree->ParseStructure() + "," + radicand->ParseStructure() + ")";
}
#endif

bool NthRootFormulaNode::FromString(std::string::iterator& begin, std::string::iterator& end, FormulaNode* parent)
{
	std::string::iterator i = begin;
	
	if (FindSubstring(begin, end, "root"))
	{
		NthRootFormulaNode* t = new NthRootFormulaNode(parent, parent->wnd);
		i += 4;
		
		if (!GroupFormulaNode::FromString(i, end, parent))
		{
			delete t;
			return false;
		}
		
		FormulaNode* g = (*parent->childNodes)[parent->ChildrenCount() - 1];
		FormulaNode* c = t->degree;
		for (int j = 0, k = 0; j < g->ChildrenCount();)
		{
			FormulaNode* n = (*g)[j];
			if (n->type == COMMA_NODE)
			{
				++j;
				k = 0;
				c = t->radicand;
				continue;
			}
			c->MoveChild(n, k++);
		}
		
		parent->RemoveChild(parent->ChildrenCount() - 1);
		
		if (t->degree->ChildrenCount() == 0)
			t->degree->AddChild(new EmptyFormulaNode(t->degree));
		if (t->radicand->ChildrenCount() == 0)
			t->radicand->AddChild(new EmptyFormulaNode(t->radicand));
		
		parent->AddChild(t);
		
		begin = i;
		return true;
	}
	
	return false;
}

std::string NthRootFormulaNode::ToString()
{
	return "root(" + degree->ToString() + "," + radicand->ToString() + ")";
}

void NthRootFormulaNode::RenderCaret(const int pos, const int anchor)
{
	if (pos == 1)
	{
		//draw the caret on the shape
		QRectF r = GetDocumentPosBounds(pos);
		int cx = GetExpression(1)->boundingRect.height() * 5 / 11;
		
		QGraphicsItemGroup* g = wnd->GetCaret()->caretShape;
		
		qDeleteAll(g->childItems());
		g->childItems().clear();
		
		QGraphicsLineItem* i = new QGraphicsLineItem(r.left(), r.top(), r.left(), r.bottom(), g);
		i->setPen(QPen("red"));
		g->addToGroup(i);
		i = new QGraphicsLineItem(r.left(), r.bottom(), r.left() + cx, r.bottom(), g);
		i->setPen(QPen("red"));
		g->addToGroup(i);
	}
	else
		CompoundFormulaNode::RenderCaret(pos, anchor);
}

bool NthRootFormulaNode::DoRemoveItem(Command* command)
{
	command->SaveNodeState(parent);
	
	SharedCaretState c = SharedCaretState(command->beforeCaretState->Dublicate());
	int pos = c->GetPos();
	
	if (pos == 1)
	{
		//removing the symbol
		bool right = any_cast<bool>(command->nodeEvent["right"]);
		if (!right)
			return false;
		
		int j = parent->GetFirstLevelChildPos(this);
		int i = degree->ChildrenCount();
		int k = radicand->ChildrenCount();
		parent->MoveChildren(degree, 0, i, j);
		parent->MoveChildren(radicand, 0, k, i + j);
		c->SetToNode(parent, i);
		command->afterCaretState = c;
		parent->RemoveChild(j + i + k);
		return true;
	}

	return FormulaNode::DoRemoveItem(command);
}
