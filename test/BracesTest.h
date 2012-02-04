#ifndef BRACESTEST_H
#define BRACESTEST_H
#ifdef TEST

#include <QObject>
#include "NcTest.h"

/**
 * Tests for the braces formula node.
 */
class BracesTest : public NcTest
{
	Q_OBJECT
	
public:
	/**
	 * Constructor.
	 * @param [in] _mainWindow The main window.
	 */
	BracesTest(MainWindow* _mainWindow) : NcTest(_mainWindow)
	{
	}

private slots:
	void initTestCase()
	{
		QTest::qWaitForWindowShown(wnd);
	}
	
	void Test1();
	void Test2();
	void Test3();
	void Test4();
	void Test5();
};

#endif
#endif
