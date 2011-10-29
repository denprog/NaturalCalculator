#include "NaturalCalculatorApp.h"

/**
 * Constructor.
 * @param [in] argc The argc.
 * @param [in] argv The argv.
 */
NaturalCalculatorApp::NaturalCalculatorApp(int& argc, char** argv) : QApplication(argc, argv)
{
}

/**
 * Destructor.
 */
NaturalCalculatorApp::~NaturalCalculatorApp()
{
}

/**
 * Adds a formula window.
 * @param [in,out] wnd If non-null, the window.
 */
void NaturalCalculatorApp::AddFormulaWnd(FormulaWnd* wnd)
{
	formulaWnds.push_back(wnd);
}

/**
 * Gets a formula window.
 * @param pos The position.
 * @return null if it fails, else the formula window.
 */
FormulaWnd* NaturalCalculatorApp::GetFormulaWnd(int pos)
{
	return formulaWnds[pos];
}

/**
 * Gets a formula window position in the formulaWnds.
 * @param [in,out] The window.
 * @return The formula window position.
 */
int NaturalCalculatorApp::GetFormulaWndPos(FormulaWnd* wnd)
{
	return find(formulaWnds.begin(), formulaWnds.end(), wnd) - formulaWnds.begin();
}
