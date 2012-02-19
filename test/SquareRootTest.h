#ifdef TEST
#ifndef SQUAREROOTTEST_H
#define SQUAREROOTTEST_H

#include <QObject>
#include "NcTestBase.h"

/**
 * Tests for the square root node.
 */
class SquareRootTest : public NcTestBase
{
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	SquareRootTest(MainWindow* _mainWindow) : NcTestBase(_mainWindow)
	{
	}

public:
	void Test1();
	void Test2();
	void Test3();
};

#endif
#endif
