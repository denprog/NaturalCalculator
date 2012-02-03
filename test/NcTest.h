#ifdef TEST
#ifndef NCTEST_H
#define NCTEST_H

#include <QtTest/QtTest>
#include "../../source/Main/MainWindow.h"
#include "../../source/Main/FormulaWnd.h"

class NcTest : public QObject
{
public:
	NcTest(MainWindow* _mainWindow) : mainWindow(_mainWindow), wnd(_mainWindow->formulaWnd)
	{
	}

protected:
	void Check(QString actual, QString expected, int caretPos);
	
protected:
	void MoveLeft(int count = 1);
	void MoveRight(int count = 1);
	
	void UndoKeys();
	void RedoKeys();
	
protected:
	MainWindow* mainWindow;
	FormulaWnd* wnd;
};

#endif
#endif
