#ifndef SETTINGSDIALOGPAGES_H
#define SETTINGSDIALOGPAGES_H

#include <QWidget>
#include <QButtonGroup>
#include <QGroupBox>
#include <QCheckBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QListWidget>
#include "Settings.h"

/**
 * Formula fonts page.
 */
class FormulaFontsPage : public QWidget
{
public:
	FormulaFontsPage(Settings* _settings, QWidget* parent = 0);

public:
	void Store();
	
private:
	Settings* settings;
	
	struct FontGroup
	{
		QGroupBox* fontGroup;
		QCheckBox* italicCheck;
		QCheckBox* boldCheck;
		QCheckBox* underlineCheck;
		QFontComboBox* fontNameCombo;
		QSpinBox* fontSizeSpin;
	};
	
	FontGroup fontGroups[3]; //widget groups for setting font properties
};

class ColorsComboBox : public QComboBox
{
public:
	ColorsComboBox(QWidget* parent = NULL);
	
public:
	QString Color();
	void SetColor(QString& color);

private:
	void PopulateList();
};

/**
 * Formula colors page.
 */
class FormulaColorsPage : public QWidget
{
public:
	FormulaColorsPage(Settings* _settings, QWidget* parent = 0);

public:
	void Store();
	
private:
	Settings* settings;
	
	ColorsComboBox* numbersColor;
	ColorsComboBox* variablesColor;
	ColorsComboBox* functionsColor;
	ColorsComboBox* unitsColor;
	ColorsComboBox* graphicalElementsColor;
	ColorsComboBox* errorColor;
	ColorsComboBox* solvingHighlightColor;
	ColorsComboBox* textColor;
	ColorsComboBox* cursorColor;
	ColorsComboBox* cursorHighlightColor;
	ColorsComboBox* backgroundColor;
};

class MathResultPage : public QWidget
{
	Q_OBJECT
	
public:
	MathResultPage(Settings* _settings, QWidget* parent = 0);

public:
	void Store();

private slots:
	void OnUpButtonClicked();
	void OnDownButtonClicked();
	
private:
	class ResultsListWidget : public QListWidget
	{
	public:
		virtual QSize sizeHint() const
		{
			QSize s;
			s.setWidth(sizeHintForColumn(0));
			s.setHeight(sizeHintForRow(0));
			return s;
		}
	};
	
private:
	Settings* settings;
	
	QCheckBox* resultAutoAdvanceCheck;
	ResultsListWidget* resultsOrderList;
	
	QSpinBox* scientificResultAccuracySpin;
	QSpinBox* scientificThresholdSpin;
	QCheckBox* showAngleMeasureCheck;
	QComboBox* scientificAngleMeasureCombo;
	
	QComboBox* integerNotationCombo;
	QCheckBox* showNotationCheck;
	
	QComboBox* rationalFormCombo;
};

#endif
