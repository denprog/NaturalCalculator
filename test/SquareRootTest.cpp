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
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QTest::keyClicks(wnd, "123\\");
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(123)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 3);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 3);
	
	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(123)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 3);
	
	QTest::keyClicks(wnd, "+23-4");
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(123+23-4)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(123+)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 2);

	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
}

/**
 * Test with nested square roots.
 */
void SquareRootTest::Test2()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "\\\\");
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(sqrt(g(e)))))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(sqrt(g(e)))))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
}

/**
 * Test with a division.
 */
void SquareRootTest::Test3()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "\\\\2/4");
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(sqrt(g((g(2)/g(4)))))))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(sqrt(g(e)))))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(sqrt(g((g(2)/g(4)))))))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	QTest::keyClick(wnd, Qt::Key_Home);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QCOMPARE(doc->ParseStructure().c_str(), "g(sqrt(g(e)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	wnd->Redo();
	wnd->Redo();
}

#endif
