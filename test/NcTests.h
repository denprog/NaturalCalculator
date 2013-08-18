#ifdef TEST
#ifndef NCTESTS_H
#define NCTESTS_H

#include <QtTest/QtTest>
#include "../source/Main/MainWindow.h"

/**
 * Tests for NC.
 */
class NcTests : public QObject
{
	Q_OBJECT
	
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	NcTests(MainWindow* _mainWindow) : mainWindow(_mainWindow)
	{
	}
	
private slots:
	void initTestCase()
	{
		QTest::qWaitForWindowShown(mainWindow->formulaWnd);
	}

	void TextTests();
	void BracesTests();
	void PowerTests();
	void SquareRootTests();
	void EquationTests();
	void ParseTests();
	void DocumentTests();

private:
	MainWindow* mainWindow;
};

#endif
#endif
