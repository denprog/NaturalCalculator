#ifdef TEST
#include "BracesTest.h"

/**
 * Testing of the left brace with a simple row
 */
void BracesTest::Test1()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "1+2+3+4");
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4[sg(e))", 0);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	RedoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4[sg(e))", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+[sg(4))", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	RedoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+[sg(4))", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g([sg(1+2+3+4))", 0);
	s = "";
}

/**
 * Testing of the right brace with a simple row
 */
void BracesTest::Test2()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "1+2+3+4");
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(1+2+3+4)s])", 1);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	RedoKeys();
	doc->ParseStructure(s);
	Check(s, "g(g(1+2+3+4)s])", 1);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(1+2+3+)s]4)", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	RedoKeys();
	doc->ParseStructure(s);
	Check(s, "g(g(1+2+3+)s]4)", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(e)s]1+2+3+4)", 0);
	s = "";
}

/**
 * Testing of the both left and right braces with a simple row
 */
void BracesTest::Test3()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "1+2+3+4");
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft(7);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(1+[sg(2+3+4))", 0);
	s = "";
	
	MoveRight(5);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(1+[sg(2+3)s]+4)", 3);
	s = "";
}

/**
 * Tests for several braces.
 */
void BracesTest::Test4()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "12+234");
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 3);
	s = "";

	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12+234)s])s])", 1);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 3);
	s = "";

	MoveLeft(3);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12+)s])s]234)", 0);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 0);
	s = "";

	MoveLeft(4);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(e)s])s]12+234)", 0);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 0);
	s = "";
	
	MoveRight(4);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(12+[sg([sg(234)))", 0);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 0);
	s = "";
	
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s])s]+234)", 1);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 2);
	s = "";
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(12[sg([sg(+234)))", 0);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 1);
	s = "";

	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12+)s])s]234)", 1);
	s = "";
	
	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 0);
	s = "";
}

/**
 * Tests for compound nodes.
 */
void BracesTest::Test5()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "12/234");
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234)))", 3);
	s = "";
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(g(12)s/g(234)))s])", 1);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234)))", 1);
	s = "";
	
	MoveLeft(9);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g([sg(g(g(12)s/g(234))))", 0);
	s = "";
	
	MoveRight(4);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g([sg(g(g(g(12)s])s/g(234))))", 1);
	s = "";

	UndoKeys();
	MoveRight(6);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g([sg(g(g(12)s/g(234)))s])", 1);
	s = "";

	UndoKeys();
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g([sg(g(g(12)s/g(234)))s])", 1);
	s = "";
	
	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234)))", 0);
	s = "";
	
	MoveRight(10);
	QTest::keyClicks(wnd, "+\\567");
	MoveLeft(5);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234))+[sg(sqrt(g(567))))", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234))+sqrt(g(567)))", 2);
	s = "";
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234))+sqrt(g(567)))", 0);
	s = "";
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234))+sqrt(g([sg(567))))", 0);
	s = "";
	
	MoveRight(3);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234))+sqrt(g([sg(567)s])))", 1);
	s = "";
	
	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)s/g(234))+sqrt(g(567)))", 0);
	s = "";
}

#endif
