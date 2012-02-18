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

#endif
