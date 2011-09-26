#include "NaturalCalculatorApp.h"
#include "MainWindow.h"
#include <QtGui/QApplication>
//#include <Parser.h>

int main(int argc, char *argv[])
{
	NaturalCalculatorApp app(argc, argv);
	
	MainWindow mainWindow;
	app.AddFormulaWnd(mainWindow.formulaWnd);
	mainWindow.show();
	
	return app.exec();
}
