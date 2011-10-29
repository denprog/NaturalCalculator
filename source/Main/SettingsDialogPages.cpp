#include <QtGui>
#include "SettingsDialogPages.h"
#include "../FormulaNodes/FormulaNode.h"

//FormulaFontsPage

/**
 * Constructor.
 * @param [in] _settings The application settings.
 * @param [in] parent	The parent widget.
 */
FormulaFontsPage::FormulaFontsPage(Settings* _settings, QWidget* parent) : QWidget(parent), settings(_settings)
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
		
	fontGroups[0].fontGroup = new QGroupBox(tr("Standard font"));
	fontGroups[1].fontGroup = new QGroupBox(tr("First level index font"));
	fontGroups[2].fontGroup = new QGroupBox(tr("Second level index font"));
	
	//fill the font group widgets
	for (int i = 0; i < 3; ++i)
	{
		QFont& font = settings->GetTextFormulaNodeFont((FormulaNodeLevel)(i + 1));
		
		FontGroup& f = fontGroups[i];
		f.italicCheck = new QCheckBox(tr("Italic"));
		f.italicCheck->setChecked(font.italic());
		f.boldCheck = new QCheckBox(tr("Bold"));
		f.boldCheck->setChecked(font.bold());
		f.underlineCheck = new QCheckBox(tr("Underline"));
		f.underlineCheck->setChecked(font.underline());
		f.fontNameCombo = new QFontComboBox();
		f.fontNameCombo->setCurrentFont(font);
		f.fontSizeSpin = new QSpinBox();
		f.fontSizeSpin->setMinimum(1);
		f.fontSizeSpin->setMaximum(100);
		f.fontSizeSpin->setValue(font.pointSize());

		QVBoxLayout* vLayout = new QVBoxLayout;
		vLayout->addWidget(f.italicCheck);
		vLayout->addWidget(f.boldCheck);
		vLayout->addWidget(f.underlineCheck);

		QHBoxLayout* hLayout = new QHBoxLayout;
		hLayout->addWidget(f.fontNameCombo);
		hLayout->addWidget(f.fontSizeSpin);

		QHBoxLayout* configLayout = new QHBoxLayout;
		configLayout->addLayout(vLayout);
		configLayout->addLayout(hLayout);
		configLayout->setAlignment(hLayout, Qt::AlignTop);
		f.fontGroup->setLayout(configLayout);
		
		mainLayout->addWidget(f.fontGroup);
	}

	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

/**
 * Stores data from this page to the settings.
 */
void FormulaFontsPage::Store()
{
	for (int i = 0; i < 3; ++i)
	{
		QFont& f = settings->GetTextFormulaNodeFont((FormulaNodeLevel)(i + 1));
		f.setBold(fontGroups[i].boldCheck->isChecked());
		f.setItalic(fontGroups[i].italicCheck->isChecked());
		f.setUnderline(fontGroups[i].underlineCheck->isChecked());
		f.setPointSize(fontGroups[i].fontSizeSpin->value());
		f.setFamily(fontGroups[i].fontNameCombo->currentFont().family());
	}
}

//FormulaColorsPage

FormulaColorsPage::FormulaColorsPage(QWidget* parent) : QWidget(parent)
{
}
