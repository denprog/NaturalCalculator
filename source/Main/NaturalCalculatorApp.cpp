#include "NaturalCalculatorApp.h"

NaturalCalculatorApp::NaturalCalculatorApp(int& argc, char** argv) : QApplication(argc, argv)
{
}

NaturalCalculatorApp::~NaturalCalculatorApp()
{
}

void NaturalCalculatorApp::AddFormulaWnd(FormulaWnd* wnd)
{
	formulaWnds.push_back(wnd);
}

FormulaWnd* NaturalCalculatorApp::GetFormulaWnd(int pos)
{
	return formulaWnds[pos];
}

int NaturalCalculatorApp::GetFormulaWndPos(FormulaWnd* wnd)
{
	return find(formulaWnds.begin(), formulaWnds.end(), wnd) - formulaWnds.begin();
}
