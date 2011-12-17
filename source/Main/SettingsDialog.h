#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "Settings.h"

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class FormulaFontsPage;
class FormulaColorsPage;
class MathResultPage;

/**
 * The settings dialog.
 */
class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(Settings* _settings);
	
public slots:
	void OnOk();

private:
	Settings* settings;
	FormulaFontsPage* formulaFontsPage;
	FormulaColorsPage* formulaColorsPage;
	MathResultPage* mathResultPage;
};

#endif
