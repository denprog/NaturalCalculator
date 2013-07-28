#ifdef TEST
#include "TextTest.h"

/**
 * Tests with one node.
 */
void TextTest::Test1()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "123");
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(12)", 2);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1)", 1);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(1)", 1);
	s = "";
	
	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(12)", 2);
	s = "";

	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(12)", 2);
	s = "";

	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	doc->ParseStructure(s);
	Check(s, "g(12)", 2);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Left);
	doc->ParseStructure(s);
	Check(s, "g(12)", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(1)", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
}

/**
 * Tests with a row of nodes.
 */
void TextTest::Test2()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "123+56");
	doc->ParseStructure(s);
	Check(s, "g(123+56)", 2);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	QTest::keyClick(wnd, Qt::Key_Backspace);
	doc->ParseStructure(s);
	Check(s, "g(123+)", 2);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	doc->ParseStructure(s);
	Check(s, "g(123)", 1);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(123+)", 2);
	s = "";
	
	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(123+56)", 2);
	s = "";
	
	MoveLeft(7);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(+56)", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(3+56)", 0);
	s = "";
	
	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(123+56)", 0);
	s = "";

	QTest::keyClick(wnd, Qt::Key_Right);
	QTest::keyClick(wnd, Qt::Key_Right);
	QTest::keyClicks(wnd, "-4.3");
	doc->ParseStructure(s);
	Check(s, "g(123-4.3+56)", 3);
	s = "";

	QTest::keyClicks(wnd, "*2");
	doc->ParseStructure(s);
	Check(s, "g(123-4.3*2+56)", 1);
	s = "";
}

void TextTest::Test3()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "123/");
	doc->ParseStructure(s);
	Check(s, "g((g(123)/g(e)))", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";

	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g((g(123)/g(e)))", 0);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
	
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	QTest::keyClicks(wnd, "2.3^");
	doc->ParseStructure(s);
	Check(s, "g(pow(g(2.3),g(e)))", 0);
	s = "";
}

#endif
