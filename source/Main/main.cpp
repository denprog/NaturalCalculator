#include "NaturalCalculatorApp.h"
#include "MainWindow.h"
#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#ifdef TEST

#include <QtTest/QtTest>
//#include "../../test/TextTest.h"
//#include "../../test/BracesTest.h"
#include "../../test/NcTests.h"

int main(int argc, char *argv[])
{
	NaturalCalculatorApp app(argc, argv);
	
#ifdef RU_RU_LOCALE
	QTranslator appTranslator;
	appTranslator.load("../../source/Translation/naturalcalculator_ru");
	app.installTranslator(&appTranslator);
#endif

	MainWindow mainWindow;
	app.AddFormulaWnd(mainWindow.formulaWnd);
	mainWindow.show();

	//TextTest textTest(&mainWindow);
	//QTest::qExec(&textTest, argc, argv);
	//BracesTest bracesTest(&mainWindow);
	//QTest::qExec(&bracesTest);
	NcTests ncTest(&mainWindow);
	QTest::qExec(&ncTest, argc, argv);
	
	return 0;
}
#else
/**
 * Main entry-point for this application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Exit-code for the process - 0 for success, else an error code.
 */
int main(int argc, char *argv[])
{
	NaturalCalculatorApp app(argc, argv);
	
#ifdef RU_RU_LOCALE
	QTranslator appTranslator;
	appTranslator.load("../../source/Translation/naturalcalculator_ru");
	app.installTranslator(&appTranslator);
#endif

	MainWindow mainWindow;
	app.AddFormulaWnd(mainWindow.formulaWnd);
	mainWindow.show();
	
	return app.exec();
}
#endif
