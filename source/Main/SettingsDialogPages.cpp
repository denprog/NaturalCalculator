#include <QtGui>
#include "SettingsDialogPages.h"

FormulaFontsPage::FormulaFontsPage(QWidget* parent) : QWidget(parent)
{
	QGroupBox* fontGroup = new QGroupBox(tr("Font"));
	
	QCheckBox* italicCheck = new QCheckBox(tr("Italic"));
	QCheckBox* boldCheck = new QCheckBox(tr("Bold"));
	QCheckBox* underlineCheck = new QCheckBox(tr("Underline"));
	
	QComboBox* fontNameCombo = new QComboBox();
	
	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(italicCheck);
	vLayout->addWidget(boldCheck);
	vLayout->addWidget(underlineCheck);
	
	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->addWidget(new QLabel(tr("Font name:")));
	hLayout->addWidget(fontNameCombo);

	QHBoxLayout* configLayout = new QHBoxLayout;
	configLayout->addLayout(vLayout);
	configLayout->addLayout(hLayout);
	configLayout->setAlignment(hLayout, Qt::AlignTop);
	fontGroup->setLayout(configLayout);
	
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(fontGroup);
	
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

FormulaColorsPage::FormulaColorsPage(QWidget* parent) : QWidget(parent)
{
}
