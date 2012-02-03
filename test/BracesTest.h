#ifndef BRACESTEST_H
#define BRACESTEST_H
#ifdef TEST

#include <QObject>
#include "NcTest.h"

class BracesTest : public NcTest
{
	Q_OBJECT
	
public:
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
};

#endif
#endif
