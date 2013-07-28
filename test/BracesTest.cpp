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
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4[g(e))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4[g(e))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+[g(4))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	mainWindow->OnUndo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+[g(4))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QCOMPARE(doc->ParseStructure().c_str(), "g([g(1+2+3+4))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
}

/**
 * Testing of the right brace with a simple row
 */
void BracesTest::Test2()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "1+2+3+4");
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(1+2+3+4)])");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(1+2+3+4)])");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	MoveLeft();
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(1+2+3+)]4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(1+2+3+)]4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	MoveLeft(10);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(e)]1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
}

/**
 * Testing of the both left and right braces with a simple row
 */
void BracesTest::Test3()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "1+2+3+4");
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+2+3+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	MoveLeft(7);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+[g(2+3+4))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	MoveRight(5);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(1+[g(2+3)]+4)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 3);
}

/**
 * Tests for several braces.
 */
void BracesTest::Test4()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "12+234");
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 3);

	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(g(12+234)])])");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 3);

	MoveLeft(3);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(g(12+)])]234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	MoveLeft(4);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(g(e)])]12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	MoveRight(4);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+[g([g(234)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(2);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(g(12)])]+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 2);
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QCOMPARE(doc->ParseStructure().c_str(), "g(12[g([g(+234)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);

	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g(g(12+)])]234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12+234)");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
}

/**
 * Tests for compound nodes.
 */
void BracesTest::Test5()
{
	QString s;

	wnd->New();
	QTest::keyClicks(wnd, "12/234");
	QCOMPARE(doc->ParseStructure().c_str(), "g((g(12)/g(234)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 3);
	
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g(g((g(12)/g(234)))])");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g((g(12)/g(234)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	MoveLeft(9);
	QTest::keyClick(wnd, Qt::Key_ParenLeft);
	QCOMPARE(doc->ParseStructure().c_str(), "g([g((g(12)/g(234))))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
	
	MoveRight(4);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	MoveLeft(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g([g((g(g(12)])/g(234))))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Undo();
	MoveRight(6);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g([g((g(12)/g(234)))])");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);

	wnd->Undo();
	MoveRight(1);
	QTest::keyClick(wnd, Qt::Key_ParenRight);
	QCOMPARE(doc->ParseStructure().c_str(), "g([g((g(12)/g(234)))])");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 1);
	
	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g((g(12)/g(234)))");
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), 0);
}

#endif
