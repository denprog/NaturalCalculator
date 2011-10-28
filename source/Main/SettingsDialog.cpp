#include <QtGui>
#include "SettingsDialog.h"
#include "SettingsDialogPages.h"

SettingsDialog::SettingsDialog(Settings* _settings) : settings(_settings)
{
	contentsWidget = new QTreeWidget;
	contentsWidget->setColumnCount(1);
	contentsWidget->setHeaderHidden(true);
	contentsWidget->setMaximumWidth(128);
	
	//fill the contents widget
	QTreeWidgetItem* i = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("System")));
	contentsWidget->addTopLevelItem(i);
	
	i = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Formula")));
	QTreeWidgetItem* j = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Fonts")));
	i->addChild(j);
	j->setData(0, Qt::UserRole, QVariant(0));
	j = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Colors")));
	i->addChild(j);
	j->setData(0, Qt::UserRole, QVariant(1));
	contentsWidget->addTopLevelItem(i);

	i = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Math")));
	contentsWidget->addTopLevelItem(i);

	contentsWidget->expandAll();
	
	//fill the pages widget
	pagesWidget = new QStackedWidget;
	formulaFontsPage = new FormulaFontsPage(settings);
	pagesWidget->addWidget(formulaFontsPage);
	pagesWidget->addWidget(new FormulaColorsPage);

	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(contentsWidget);
	horizontalLayout->addWidget(pagesWidget, 1);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	buttonBox->setCenterButtons(true);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(OnOk()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(horizontalLayout);
	mainLayout->addStretch(1);
	mainLayout->addSpacing(12);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);

	connect(contentsWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(OnChangePage(QTreeWidgetItem*, QTreeWidgetItem*)));

	setWindowTitle(tr("Settings"));
}

void SettingsDialog::OnChangePage(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if (!current)
		current = previous;

	QVariant v = current->data(0, Qt::UserRole);
	if (v.type() == QVariant::Int)
	{
		//switch to a new page
		pagesWidget->setCurrentIndex(v.toInt());
	}
	else
	{
		//pass this item becouse not having a data
		QTreeWidgetItem* i = contentsWidget->itemBelow(current);
		if (i)
			contentsWidget->setCurrentItem(i);
	}
}

void SettingsDialog::OnOk()
{
	formulaFontsPage->Store();
	
	accept();
}
