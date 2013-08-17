#ifdef TEST
#ifndef BRACESTEST_H
#define BRACESTEST_H

#include <QObject>
#include "NcTestBase.h"

/**
 * Tests for the braces formula node.
 */
class BracesTest : public NcTestBase
{
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	BracesTest(MainWindow* _mainWindow) : NcTestBase(_mainWindow)
	{
	}

public:
	void Test1();
	void Test2();
	void Test3();
	void Test4();
	void Test5();
	void Test6();
};

#endif
#endif
