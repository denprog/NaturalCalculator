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

class SettingsDialog : public QDialog
{
	Q_OBJECT
	
public:
	SettingsDialog(Settings* _settings);
	
public slots:
	void OnChangePage(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void OnOk();

private:
	Settings* settings;
	QTreeWidget *contentsWidget;
	QStackedWidget *pagesWidget;
	FormulaFontsPage* formulaFontsPage;
};

#endif
