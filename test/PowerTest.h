#ifdef TEST
#ifndef POWERTEST_H
#define	POWERTEST_H

#include <QObject>
#include "NcTestBase.h"

/**
 * Tests for the power node.
 */
class PowerTest : public NcTestBase
{
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	PowerTest(MainWindow* _mainWindow) : NcTestBase(_mainWindow)
	{
	}

public:
	void Test1();
};

#endif
#endif
