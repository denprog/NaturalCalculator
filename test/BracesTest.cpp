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
	Check(s, "g(1+2+3+4[g(e))", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4[g(e))", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+[g(4))", 0);
	s = "";

	mainWindow->OnUndo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+[g(4))", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g([g(1+2+3+4))", 0);
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
	Check(s, "g(g(1+2+3+4)])", 1);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(g(1+2+3+4)])", 1);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(1+2+3+)]4)", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(g(1+2+3+)]4)", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(e)]1+2+3+4)", 0);
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
	Check(s, "g(1+[g(2+3+4))", 0);
	s = "";
	
	MoveRight(5);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(1+[g(2+3)]+4)", 3);
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
	Check(s, "g(g(g(12+234)])])", 1);
	s = "";

	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 3);
	s = "";

	MoveLeft(3);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12+)])]234)", 0);
	s = "";

	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 0);
	s = "";

	MoveLeft(4);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(e)])]12+234)", 0);
	s = "";

	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 0);
	s = "";
	
	MoveRight(4);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(12+[g([g(234)))", 0);
	s = "";

	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 0);
	s = "";
	
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g(g(12)])]+234)", 1);
	s = "";

	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(12+234)", 2);
	s = "";
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g(12[g([g(+234)))", 0);
	s = "";

	wnd->Undo();
	wnd->Undo();
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
	Check(s, "g(g(g(12+)])]234)", 1);
	s = "";
	
	wnd->Undo();
	wnd->Undo();
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
	Check(s, "g((g(12)/g(234)))", 3);
	s = "";
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g(g((g(12)/g(234)))])", 1);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g((g(12)/g(234)))", 1);
	s = "";
	
	MoveLeft(9);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	doc->ParseStructure(s);
	Check(s, "g([g((g(12)/g(234))))", 0);
	s = "";
	
	MoveRight(4);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g([g((g(g(12)])/g(234))))", 1);
	s = "";

	wnd->Undo();
	MoveRight(6);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g([g((g(12)/g(234)))])", 1);
	s = "";

	wnd->Undo();
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	doc->ParseStructure(s);
	Check(s, "g([g((g(12)/g(234)))])", 1);
	s = "";
	
	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g((g(12)/g(234)))", 0);
	s = "";
}

#endif
