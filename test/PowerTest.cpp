#ifdef TEST
#include "PowerTest.h"

void PowerTest::Test1()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "^");
	doc->ParseStructure(s);
	Check(s, "g(pow(g(e),g(e)))", 0);
	s = "";
	
	wnd->Undo();
	doc->ParseStructure(s);
	Check(s, "g(e)", 0);
	s = "";

	QTest::keyClicks(wnd, "12^");
	doc->ParseStructure(s);
	Check(s, "g(pow(g(12),g(e)))", 0);
	s = "";

	QTest::keyClicks(wnd, "3");
	doc->ParseStructure(s);
	Check(s, "g(pow(g(12),g(3)))", 1);
	s = "";
}

#endif
