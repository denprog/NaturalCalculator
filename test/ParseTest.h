#ifdef TEST
#ifndef PARSETEST_H
#define	PARSETEST_H

#include "NcTestBase.h"

class ParseTest : public NcTestBase
{
public:
	ParseTest(MainWindow* _mainWindow) : NcTestBase(_mainWindow)
	{
	}

public:
	void Test1();
};

#endif
#endif
