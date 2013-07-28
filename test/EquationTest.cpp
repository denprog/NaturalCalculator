#ifdef TEST
#include "EquationTest.h"

void EquationTest::Test1()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "=");
	doc->ParseStructure(s);
	Check(s, "g(g(e)=g(g()))", 1);
	s = "";
}

#endif

