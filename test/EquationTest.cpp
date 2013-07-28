#ifdef TEST
#include "EquationTest.h"

void EquationTest::Test1()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "=");
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(e)=g(g()))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
}

#endif

