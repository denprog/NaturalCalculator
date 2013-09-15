#ifdef TEST
#include "TextTest.h"

/**
 * Tests with one node.
 */
void TextTest::Test1()
{
	wnd->New();
	QTest::keyClicks(wnd, "123");
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 3);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
	
	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(1)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
	
	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 3);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(12)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 3);
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	QCOMPARE(doc->ParseStructure().c_str(), "g(12)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	QTest::keyClick(wnd, Qt::Key_Left);
	QCOMPARE(doc->ParseStructure().c_str(), "g(12)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
	
	QTest::keyClick(wnd, Qt::Key_Delete);
	QCOMPARE(doc->ParseStructure().c_str(), "g(1)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
}

/**
 * Tests with a row of nodes.
 */
void TextTest::Test2()
{
	wnd->New();
	QTest::keyClicks(wnd, "123+56");
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+56)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	QTest::keyClick(wnd, Qt::Key_Backspace);
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	QTest::keyClick(wnd, Qt::Key_Backspace);
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+56)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 2);
	
	MoveLeft(7);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QTest::keyClick(wnd, Qt::Key_Delete);
	QCOMPARE(doc->ParseStructure().c_str(), "g(+56)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(3+56)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
	
	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123+56)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);

	QTest::keyClick(wnd, Qt::Key_Right);
	QTest::keyClick(wnd, Qt::Key_Right);
	QTest::keyClicks(wnd, "-4.3");
	QCOMPARE(doc->ParseStructure().c_str(), "g(123-4.3+56)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 3);

	QTest::keyClicks(wnd, "*2");
	QCOMPARE(doc->ParseStructure().c_str(), "g(123-4.3*2+56)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 1);
}

void TextTest::Test3()
{
	wnd->New();
	QTest::keyClicks(wnd, "123/");
	QCOMPARE(doc->ParseStructure().c_str(), "g((g(123)/g(e)))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);

	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 3);

	wnd->Redo();
	QCOMPARE(doc->ParseStructure().c_str(), "g((g(123)/g(e)))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
	
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(123)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 3);
	
	wnd->Undo();
	wnd->Undo();
	wnd->Undo();
	QCOMPARE(doc->ParseStructure().c_str(), "g(e)");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
	
	QTest::keyClicks(wnd, "2.3^");
	QCOMPARE(doc->ParseStructure().c_str(), "g(pow(g(2.3),g(e)))");
	QCOMPARE(wnd->caret->currentState->GetPos(), 0);
}

#endif
