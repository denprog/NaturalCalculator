#ifndef NATURALCALCULATORAPP_H
#define NATURALCALCULATORAPP_H

#include <QApplication>
#include <vector>
#include "FormulaWnd.h"

using namespace std;

/**
 * Natural calculator application.
 */
class NaturalCalculatorApp : public QApplication
{
public:
	NaturalCalculatorApp(int& argc, char** argv);
	virtual ~NaturalCalculatorApp();
	
public:
	void AddFormulaWnd(FormulaWnd* wnd);
	FormulaWnd* GetFormulaWnd(int pos);
	int GetFormulaWndPos(FormulaWnd* wnd);
	
private:
	vector<FormulaWnd*> formulaWnds; ///< The formula windows
};

#endif
