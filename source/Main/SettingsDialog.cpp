#include <QtGui>
#include "SettingsDialog.h"
#include "SettingsDialogPages.h"

/**
 * Constructor.
 * @param [in] _settings The application settings.
 */
SettingsDialog::SettingsDialog(Settings* _settings) : settings(_settings)
{
	QTabWidget* settingsTab = new QTabWidget;
	
	systemPage = new SystemPage(settings);
	settingsTab->addTab(systemPage, tr("System"));
	
	formulaFontsPage = new FormulaFontsPage(settings);
	settingsTab->addTab(formulaFontsPage, tr("Fonts"));
	
	formulaColorsPage = new FormulaColorsPage(settings);
	settingsTab->addTab(formulaColorsPage, tr("Colors"));
	
	mathResultPage = new MathResultPage(settings);
	settingsTab->addTab(mathResultPage, "Result");

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	buttonBox->setCenterButtons(true);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(OnOk()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(settingsTab);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);

	setWindowTitle(tr("Settings"));
}

/**
 * The ok button event.
 */
void SettingsDialog::OnOk()
{
	formulaColorsPage->Store();
	formulaFontsPage->Store();
	mathResultPage->Store();
	
	accept();
}
