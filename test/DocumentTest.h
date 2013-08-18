#ifdef TEST
#ifndef DOCUMENTTEST_H
#define	DOCUMENTTEST_H

#include <QObject>
#include "NcTestBase.h"

/**
 * Tests for the document formula node.
 */
class DocumentTest : public NcTestBase
{
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	DocumentTest(MainWindow* _mainWindow) : NcTestBase(_mainWindow)
	{
	}

public:
	void Test1();
};

#endif
#endif
