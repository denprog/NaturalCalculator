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
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4[sg(e))", 0);
	s = "";
	
	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	RedoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4[sg(e))", 0);
	s = "";

	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+[sg(4))", 0);
	s = "";

	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	RedoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+[sg(4))", 0);
	s = "";

	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	wnd->GetDocumentNode()->ParseStructure(s);
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
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";
	
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(g(1+2+3+4)s])", 1);
	s = "";
	
	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	RedoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(g(1+2+3+4)s])", 1);
	s = "";

	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(g(1+2+3+)s]4)", 1);
	s = "";

	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	RedoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(g(1+2+3+)s]4)", 1);
	s = "";

	UndoKeys();
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 0);
	s = "";

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(g(e)s]1+2+3+4)", 1);
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
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+2+3+4)", 1);
	s = "";

	MoveLeft(7);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+[sg(2+3+4))", 0);
	s = "";
	
	MoveRight(5);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	wnd->GetDocumentNode()->ParseStructure(s);
	Check(s, "g(1+[sg(2+3)s]+4)", 3);
	s = "";
}

#endif
