#ifndef SETTINGSDIALOGPAGES_H
#define SETTINGSDIALOGPAGES_H

#include <QWidget>
#include "Settings.h"

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

class FormulaColorsPage : public QWidget
{
public:
	FormulaColorsPage(QWidget* parent = 0);
};

#endif
