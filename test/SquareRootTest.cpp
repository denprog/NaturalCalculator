#ifdef TEST
#include "SquareRootTest.h"

/**
 * Testing the square root with a simple row
 */
void SquareRootTest::Test1()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "\\");
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	RedoKeys();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";

	UndoKeys();
	QTest::keyClicks(wnd, "123\\");
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(123)))", 3);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
}

/**
 * Test with nested square roots.
 */
void SquareRootTest::Test2()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "\\\\");
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(sqrt(g(e)))))", 0);
	s = "";
	
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";

	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";
	
	RedoKeys();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";

	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(sqrt(g(e)))))", 0);
	s = "";
}

/**
 * Test with a division.
 */
void SquareRootTest::Test3()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "\\\\2/4");
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(sqrt(g(g(g(2)s/g(4)))))))", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(sqrt(g(e)))))", 0);
	s = "";
	
	UndoKeys();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(sqrt(g(g(g(2)s/g(4)))))))", 0);
	s = "";

	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";
	
	RedoKeys();
	RedoKeys();
}

#endif
