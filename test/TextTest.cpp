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
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	doc->ParseStructure(s);
	Check(s, "g(12)", 2);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
	
	RedoKeys();
	doc->ParseStructure(s);
	Check(s, "g(12)", 2);
	s = "";

	QTest::keyClick(wnd, Qt::Key_Backspace);
	QTest::keyClick(wnd, Qt::Key_Backspace);
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(1)", 1);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
	
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(12)", 2);
	s = "";

	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(2)", 0);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(123)", 2);
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
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(123+5)", 1);
	s = "";

	UndoKeys();
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

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(3+56)", 0);
	s = "";

	UndoKeys();
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(123+56)", 0);
	s = "";
}

#endif
