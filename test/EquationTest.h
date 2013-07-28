#ifdef TEST
#ifndef EQUATIONTEST_H
#define	EQUATIONTEST_H

#include <QObject>
#include "NcTestBase.h"

/**
 * Tests for the power node.
 */
class EquationTest : public NcTestBase
{
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	EquationTest(MainWindow* _mainWindow) : NcTestBase(_mainWindow)
	{
	}

public:
	void Test1();
};

#endif
#endif
