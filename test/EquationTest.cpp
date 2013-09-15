#ifdef TEST
#include "EquationTest.h"

void EquationTest::Test1()
{
	wnd->New();
	QTest::keyClicks(wnd, "=");
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(e)=$res(g($auto(g(Syntax error)))))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
	
	wnd->Redo();
	QTest::keyClick(wnd, Qt::Key_Left);
	QTest::keyClicks(wnd, "12");
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(12)=$res(g($auto(g(12.)))))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	QTest::keyClicks(wnd, "+");
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(12)=$res(g($auto(g(12.)))))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(12+)=$res(g($auto(g(Syntax error)))))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);

	QTest::keyClicks(wnd, "3");
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(12+3)=$res(g($auto(g(15.)))))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
}

#endif

