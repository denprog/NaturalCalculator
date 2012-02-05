#ifdef TEST
#ifndef TEXTTEST_H
#define TEXTTEST_H

#include "NcTestBase.h"

/**
 * Tests for text nodes.
 */
class TextTest : public NcTestBase
{
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	TextTest(MainWindow* _mainWindow) : NcTestBase(_mainWindow)
	{
	}

public:
	void Test1();
	void Test2();
};

#endif
#endif
