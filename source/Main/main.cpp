#include "NaturalCalculatorApp.h"
#include "MainWindow.h"
#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

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
