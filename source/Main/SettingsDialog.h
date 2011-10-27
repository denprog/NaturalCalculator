#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
	Q_OBJECT
	
public:
	SettingsDialog();
	
public slots:
	void OnChangePage(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
	QTreeWidget *contentsWidget;
	QStackedWidget *pagesWidget;
};

#endif
