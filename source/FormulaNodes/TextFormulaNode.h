#ifndef TEXTFORMULANODE_H
#define TEXTFORMULANODE_H

#include <QGraphicsTextItem>
#include "FormulaNode.h"

/**
 * Text formula node.
 */
class TextFormulaNode : public FormulaNode
{
public:
	TextFormulaNode();
	TextFormulaNode(FormulaNode* parent);
	TextFormulaNode(FormulaNode* parent, FormulaWnd* wnd);
	virtual ~TextFormulaNode();
	
private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		string str = ((TextFormulaNode*)this)->GetText().toUtf8().data();
		ar << str;
		ar << boost::serialization::base_object<FormulaNode>(*this);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		string str;
		ar >> str;
		SetText(str.c_str());
		ar >> boost::serialization::base_object<FormulaNode>(*this);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	virtual void UpdateBoundingRect();
	virtual void Remake();

	virtual FormulaNode* Clone(FormulaNode* p);
	
	void SetText(QString _text);
	QString GetText();
	//void SetFont(QFont font);
	
	virtual SharedCaretState GetFirstPosition();
	virtual SharedCaretState GetLastPosition();
	virtual SharedCaretState GetNextPosition(SharedCaretState& relativeState = SharedCaretState());
	virtual SharedCaretState GetPreviousPosition(SharedCaretState& relativeState = SharedCaretState());
	virtual SharedCaretState GetLineBegin(SharedCaretState& relativeState);
	virtual SharedCaretState GetLineEnd(SharedCaretState& relativeState);

	virtual QRectF GetDocumentPosBounds(int pos);

	virtual int GetNearestPos(qreal x, qreal y);

	virtual void RenderCaret(const int pos, const int anchor);

	virtual void Parse(ParserString& expr);
#ifdef TEST
	virtual void ParseStructure(QString& res);
#endif

	//command functions
	
	virtual bool DoInsertNode(NodeEvent& nodeEvent);
	virtual bool UndoInsertNode(NodeEvent& nodeEvent);
	virtual bool DoInsertText(NodeEvent& nodeEvent);
	virtual bool UndoInsertText(NodeEvent& nodeEvent);
	
	virtual bool DoRemoveItem(NodeEvent& nodeEvent);
	virtual bool UndoRemoveItem(NodeEvent& nodeEvent);

	virtual bool DoCreatePlusFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreatePlusFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateMinusFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateMinusFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateMultiplyFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateMultiplyFormulaNode(NodeEvent& nodeEvent);
	
	virtual bool DoCreateDivisionFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateDivisionFormulaNode(NodeEvent& nodeEvent);
	
	virtual bool DoCreatePowerFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreatePowerFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateSquareRootFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateSquareRootFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateLeftBraceFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateLeftBraceFormulaNode(NodeEvent& nodeEvent);

	virtual bool DoCreateRightBraceFormulaNode(NodeEvent& nodeEvent);
	virtual bool UndoCreateRightBraceFormulaNode(NodeEvent& nodeEvent);
	
public slots:
	virtual void OnCopy();
	virtual void OnPaste();
	virtual void OnCut();
};

class FormulaTextItem : public QGraphicsTextItem
{
public:
	FormulaTextItem(Settings* _settings, FormulaNodeLevel _level, QRectF& _boundingRect, QGraphicsItem* parent = 0);
	
public:
	friend class TextFormulaNode;
	
public:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	
private:
	Settings* settings;
	FormulaNodeLevel level;
	QRectF boundingRect;
};

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive& ar, const TextFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent = node->GetParent();
			ar << parent;
		}

		template<class Archive>
		inline void load_construct_data(Archive& ar, TextFormulaNode* node, const BOOST_PFTO unsigned int file_version)
		{
			FormulaNode* parent;
			ar >> parent;
			::new (node)TextFormulaNode(parent, parent->GetWnd());
		}
	}
}

#endif
