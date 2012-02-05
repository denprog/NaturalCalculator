#ifdef TEST
#include "NcTests.h"
#include "TextTest.h"
#include "BracesTest.h"

/**
 * Text tests.
 */
void NcTests::TextTests()
{
	TextTest textTest(mainWindow);
	textTest.Test1();
	textTest.Test2();
}

/**
 * Braces tests.
 */
void NcTests::BracesTests()
{
	BracesTest bracesTest(mainWindow);
	bracesTest.Test1();
	bracesTest.Test2();
	bracesTest.Test3();
	bracesTest.Test4();
	bracesTest.Test5();
}

#endif
