#ifdef TEST
#ifndef NCTEST_H
#define NCTEST_H

#include <QtTest/QtTest>
#include "../source/Main/MainWindow.h"
#include "../source/Main/FormulaWnd.h"

/**
 * The base class for NC's tests.
 */
class NcTestBase : public QObject
{
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	NcTestBase(MainWindow* _mainWindow) : mainWindow(_mainWindow), wnd(_mainWindow->formulaWnd), doc(wnd->GetDocumentNode())
	{
	}

protected:
	void Check(QString actual, QString expected, int caretPos);
	void Check(std::string actual, std::string& expected, int caretPos);
	
protected:
	void MoveLeft(int count = 1);
	void MoveRight(int count = 1);
	
	void UndoKeys();
	void RedoKeys();
	
protected:
	MainWindow* mainWindow;
	FormulaWnd* wnd;
	DocumentFormulaNode* doc;
};

#endif
#endif
