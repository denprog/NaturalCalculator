#ifdef TEST
#include "NcTest.h"

void NcTest::Check(QString actual, QString expected, int caretPos)
{
	QCOMPARE(actual, expected);
	QCOMPARE(wnd->GetCaret()->currentState->GetPos(), caretPos);
}

void NcTest::MoveLeft(int count)
{
	for (int i = 0; i < count; ++i)
		QTest::keyClick(wnd, Qt::Key_Left);
}

void NcTest::MoveRight(int count)
{
	for (int i = 0; i < count; ++i)
		QTest::keyClick(wnd, Qt::Key_Right);
}

void NcTest::UndoKeys()
{
	int val = QKeySequence(QKeySequence::Undo)[0];
	Qt::KeyboardModifiers mods = (Qt::KeyboardModifiers)(val & Qt::KeyboardModifierMask);
	QTest::keyClick(mainWindow, val & ~mods, mods);
}

void NcTest::RedoKeys()
{
	int val = QKeySequence(QKeySequence::Redo)[0];
	Qt::KeyboardModifiers mods = (Qt::KeyboardModifiers)(val & Qt::KeyboardModifierMask);
	QTest::keyClick(mainWindow, val & ~mods, mods);
}

#endif
