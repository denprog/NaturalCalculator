#ifndef NATURALCALCULATORAPP_H
#define NATURALCALCULATORAPP_H

#include <QApplication>
#include <vector>

class FormulaWnd;

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
	int GetFormulaWndPos(const FormulaWnd* wnd);
	
private:
	std::vector<FormulaWnd*> formulaWnds; ///< The formula windows
};

#endif
