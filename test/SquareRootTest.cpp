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
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";
	
	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";

	wnd->Undo();
	QTest::keyClicks(wnd, "123\\");
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(123)))", 3);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(123)", 3);
	s = "";
	
	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(123)))", 3);
	s = "";
	
	QTest::keyClicks(wnd, "+23-4");
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(123+23-4)))", 1);
	s = "";
	
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(123+)))", 2);
	s = "";

	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(1)", 1);
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
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";
	
	wnd->Redo();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";

	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";

	wnd->Undo();
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
	Check(s, "g(sqrt(g(sqrt(g((g(2)/g(4)))))))", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(sqrt(g(e)))))", 0);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(sqrt(g((g(2)/g(4)))))))", 0);
	s = "";

	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Delete);
	doc->ParseStructure(s);
	Check(s, "g(sqrt(g(e)))", 0);
	s = "";
	
	wnd->Redo();
	wnd->Redo();
}

#endif
