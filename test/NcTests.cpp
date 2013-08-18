#ifdef TEST
#include "NcTests.h"
#include "TextTest.h"
#include "BracesTest.h"
#include "SquareRootTest.h"
#include "ParseTest.h"
#include "PowerTest.h"
#include "EquationTest.h"
#include "DocumentTest.h"

/**
 * Text tests.
 */
void NcTests::TextTests()
{
	TextTest textTest(mainWindow);
	textTest.Test1();
	textTest.Test2();
	textTest.Test3();
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
	bracesTest.Test6();
}

void NcTests::PowerTests()
{
	PowerTest powerTest(mainWindow);
	powerTest.Test1();
}

/**
 * Square root tests.
 */
void NcTests::SquareRootTests()
{
	SquareRootTest squareRootTest(mainWindow);
	squareRootTest.Test1();
	squareRootTest.Test2();
	squareRootTest.Test3();
}

void NcTests::EquationTests()
{
	EquationTest equationTest(mainWindow);
	equationTest.Test1();
}

void NcTests::ParseTests()
{
	ParseTest parseTest(mainWindow);
	parseTest.Test1();
}

void NcTests::DocumentTests()
{
	DocumentTest documentTest(mainWindow);
	documentTest.Test1();
}

#endif
