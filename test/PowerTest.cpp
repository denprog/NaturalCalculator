#ifdef TEST
#include "PowerTest.h"

void PowerTest::Test1()
{
	wnd->New();
	QTest::keyClicks(wnd, "^");
	QCOMPARE(doc->ParseStructure().c_str(), "g(pow(g(e),g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	QTest::keyClicks(wnd, "12^");
	QCOMPARE(doc->ParseStructure().c_str(), "g(pow(g(12),g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	QTest::keyClicks(wnd, "3");
	QCOMPARE(doc->ParseStructure().c_str(), "g(pow(g(12),g(3)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	QTest::keyClick(wnd, Qt::Key_Left);
	QTest::keyClick(wnd, Qt::Key_Left);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(pow(g(12),g(3)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
}

#endif
