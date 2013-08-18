#ifdef TEST
#include "DocumentTest.h"

void DocumentTest::Test1()
{
	wnd->New();
	QTest::keyClick(wnd, Qt::Key_Enter);
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)\n\rg(e)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)\n\rg(e)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	QTest::keyClick(wnd, Qt::Key_Left);
	QTest::keyClicks(wnd, "123+456");
	QTest::keyClick(wnd, Qt::Key_Right);
	QTest::keyClicks(wnd, "34*56");
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+456)\n\rg(34*56)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 2);
	
	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Left);
	QTest::keyClick(wnd, Qt::Key_Enter);
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+456)\n\rg(e)\n\rg(34*56)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	QTest::keyClicks(wnd, "12+56=");
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+456)\n\rg(g(12+56)=$res(g($auto(g(~)))))\n\rg(34*56)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	QTest::keyClick(wnd, Qt::Key_Enter);
	QTest::keyClicks(wnd, "4/5=");
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+456)\n\rg(g(12+56)=$res(g($auto(g(~)))))\n\rg(g((g(4)/g(5)))=$res(g($auto(g(~)))))\n\rg(34*56)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+456)\n\rg(g(12+56)=$res(g($auto(g(~)))))\n\rg((g(4)/g(5)))\n\rg(34*56)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+456)\n\rg(g(12+56)=$res(g($auto(g(~)))))\n\rg(g((g(4)/g(5)))=$res(g($auto(g(~)))))\n\rg(34*56)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
}

#endif

