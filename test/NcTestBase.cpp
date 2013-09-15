#ifdef TEST
#include "NcTestBase.h"

void NcTestBase::Check(QString actual, QString expected, int caretPos)
{
	QCOMPARE(actual, expected);
	QCOMPARE(wnd->caret->currentState->GetPos(), caretPos);
}

void NcTestBase::Check(std::string actual, std::string& expected, int caretPos)
{
	Check(QString(actual.c_str()), QString(expected.c_str()), caretPos);
}

void NcTestBase::MoveLeft(int count)
{
	for (int i = 0; i < count; ++i)
		QTest::keyClick(wnd, Qt::Key_Left);
}

void NcTestBase::MoveRight(int count)
{
	for (int i = 0; i < count; ++i)
		QTest::keyClick(wnd, Qt::Key_Right);
}

void NcTestBase::UndoKeys()
{
	int val = QKeySequence(QKeySequence::Undo)[0];
	Qt::KeyboardModifiers mods = (Qt::KeyboardModifiers)(val & Qt::KeyboardModifierMask);
	QTest::keyClick(mainWindow, val & ~mods, mods);
}

void NcTestBase::RedoKeys()
{
	int val = QKeySequence(QKeySequence::Redo)[0];
	Qt::KeyboardModifiers mods = (Qt::KeyboardModifiers)(val & Qt::KeyboardModifierMask);
	QTest::keyClick(mainWindow, val & ~mods, mods);
}

#endif
