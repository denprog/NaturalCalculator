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

//MathResultPage

MathResultPage::MathResultPage(Settings* _settings, QWidget* parent) : QWidget(parent), settings(_settings)
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
	
	//results order group
	QGroupBox* advanceGroup = new QGroupBox;
	QHBoxLayout* advanceGroupLayout = new QHBoxLayout;
	
	resultAutoAdvanceCheck = new QCheckBox(tr("Result auto advance"));
	mainLayout->addWidget(resultAutoAdvanceCheck);

	resultsOrderList = new ResultsListWidget();
	resultsOrderList->setSelectionMode(QAbstractItemView::SingleSelection);
	
	for (int i = 0; i < 3; ++i)
	{
		if (i == settings->Load("ResultsOrder", "scientificResultPos", 0).toInt())
		{
			QListWidgetItem* i = new QListWidgetItem(tr("Scientific"), resultsOrderList);
			i->setData(Qt::UserRole, REAL);
			i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
			i->setCheckState(Qt::Checked);
			resultsOrderList->addItem(i);
		}
		else if (i == settings->Load("ResultsOrder", "integerResultPos", 1).toInt())
		{
			QListWidgetItem* i = new QListWidgetItem(tr("Integer"), resultsOrderList);
			i->setData(Qt::UserRole, INTEGER);
			i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
			i->setCheckState(Qt::Checked);
			resultsOrderList->addItem(i);
		}
		else if (i == settings->Load("ResultsOrder", "rationalResultPos", 2).toInt())
		{
			QListWidgetItem* i = new QListWidgetItem(tr("Rational"), resultsOrderList);
			i->setData(Qt::UserRole, RATIONAL);
			i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
			i->setCheckState(Qt::Checked);
			resultsOrderList->addItem(i);
		}
	}
	
	advanceGroupLayout->addWidget(resultsOrderList);

	QVBoxLayout* b = new QVBoxLayout;
	
	QPushButton* p = new QPushButton(QIcon(":/Resources/images/dialogs/uparrow.png"), "");
	p->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(p, SIGNAL(clicked()), this, SLOT(OnUpButtonClicked()));
	b->addWidget(p);
	
	p = new QPushButton(QIcon(":/Resources/images/dialogs/downarrow.png"), "");
	p->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(p, SIGNAL(clicked()), this, SLOT(OnDownButtonClicked()));
	b->addWidget(p);
	
	advanceGroupLayout->addLayout(b);

	advanceGroup->setLayout(advanceGroupLayout);
	
	mainLayout->addWidget(advanceGroup);

	//results tab
	QTabWidget* resultsTab = new QTabWidget;
	
	//scientific tab
	QWidget* scientificTab = new QWidget;
	
	QGridLayout* scientificTabLayout = new QGridLayout;
	
	QLabel* t = new QLabel(tr("Result accuracy:"));
	scientificTabLayout->addWidget(t, 0, 0, Qt::AlignVCenter);
	scientificResultAccuracySpin = new QSpinBox;
	scientificResultAccuracySpin->setMaximum(100);
	scientificResultAccuracySpin->setValue(settings->Load("ScientificNumbers", "resultAccuracy", 3).toInt());
	scientificTabLayout->addWidget(scientificResultAccuracySpin, 0, 1);

	t = new QLabel(tr("Exponential threshold:"));
	scientificTabLayout->addWidget(t, 1, 0, Qt::AlignVCenter);
	scientificThresholdSpin = new QSpinBox;
	scientificThresholdSpin->setMaximum(100);
	scientificThresholdSpin->setValue(settings->Load("ScientificNumbers", "exponentialThreshold", 8).toInt());
	scientificTabLayout->addWidget(scientificThresholdSpin, 1, 1);
	
	showAngleMeasureCheck = new QCheckBox(tr("Show angle measure"));
	showAngleMeasureCheck->setCheckState((Qt::CheckState)settings->Load("ScientificNumbers", "showAngleMeasure", 0).toInt());
	scientificTabLayout->addWidget(showAngleMeasureCheck, 2, 0);

	t = new QLabel(tr("Angle measure:"));
	scientificTabLayout->addWidget(t, 3, 0, Qt::AlignVCenter);
	scientificAngleMeasureCombo = new QComboBox;
	scientificAngleMeasureCombo->addItem(tr("Radian"), RADIAN);
	scientificAngleMeasureCombo->addItem(tr("Degree"), DEGREE);
	scientificAngleMeasureCombo->addItem(tr("Grad"), GRAD);
	scientificAngleMeasureCombo->setCurrentIndex(settings->Load("ScientificNumbers", "angleMeasure", 0).toInt());
	scientificTabLayout->addWidget(scientificAngleMeasureCombo, 3, 1);
	
	scientificTabLayout->setSizeConstraint(QLayout::SetFixedSize);
	scientificTab->setLayout(scientificTabLayout);
	
	resultsTab->addTab(scientificTab, tr("Scientific"));
	
	//integer tab
	QWidget* integerTab = new QWidget;
	
	QGridLayout* integerTabLayout = new QGridLayout;
	
	t = new QLabel(tr("Scale of notation:"));
	integerTabLayout->addWidget(t, 0, 0, Qt::AlignVCenter);
	
	integerNotationCombo = new QComboBox;
	integerNotationCombo->addItem(tr("Binary"), BINARY_NOTATION);
	integerNotationCombo->addItem(tr("Octonary"), OCTAL_NOTATION);
	integerNotationCombo->addItem(tr("Decimal"), DECIMAL_NOTATION);
	integerNotationCombo->addItem(tr("Hexadecimal"), HEXADECIMAL_NOTATION);
	integerNotationCombo->setCurrentIndex(settings->Load("IntegerNumbers", "notation", 3).toInt());
	integerTabLayout->addWidget(integerNotationCombo, 0, 1);

	showNotationCheck = new QCheckBox(tr("Show notation"));
	showNotationCheck->setCheckState((Qt::CheckState)settings->Load("IntegerNumbers", "showNotation", 0).toInt());
	integerTabLayout->addWidget(showNotationCheck, 1, 0);

	integerTabLayout->setSizeConstraint(QLayout::SetFixedSize);
	integerTab->setLayout(integerTabLayout);

	resultsTab->addTab(integerTab, tr("Integer"));
	
	//rational tab
	QWidget* rationalTab = new QWidget;
	
	QGridLayout* rationalTabLayout = new QGridLayout;
	
	t = new QLabel(tr("Fraction form:"));
	rationalTabLayout->addWidget(t, 0, 0, Qt::AlignVCenter);
	
	rationalFormCombo = new QComboBox;
	rationalFormCombo->addItem(tr("Proper"), PROPER_FRACTION);
	rationalFormCombo->addItem(tr("Improper"), IMPROPER_FRACTION);
	rationalFormCombo->setCurrentIndex(settings->Load("RationalNumbers", "form", 0).toInt());
	rationalTabLayout->addWidget(rationalFormCombo, 0, 1, Qt::AlignVCenter);
	
	rationalTabLayout->setSizeConstraint(QLayout::SetFixedSize);
	rationalTab->setLayout(rationalTabLayout);

	resultsTab->addTab(rationalTab, tr("Rational"));
	
	mainLayout->addWidget(resultsTab);
	
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

void MathResultPage::Store()
{
	for (int i = 0; i < resultsOrderList->count(); ++i)
	{
		QListWidgetItem* item = resultsOrderList->item(i);
		switch (item->data(Qt::UserRole).toInt())
		{
		case REAL:
			settings->Save("ResultsOrder", "scientificResultPos", i);
			break;
		case INTEGER:
			settings->Save("ResultsOrder", "integerResultPos", i);
			break;
		case RATIONAL:
			settings->Save("ResultsOrder", "rationalResultPos", i);
			break;
		}
	}
	
	settings->Save("ScientificNumbers", "resultAccuracy", scientificResultAccuracySpin->value());
	settings->Save("ScientificNumbers", "exponentialThreshold", scientificThresholdSpin->value());
	settings->Save("ScientificNumbers", "showAngleMeasure", showAngleMeasureCheck->checkState());
	settings->Save("ScientificNumbers", "angleMeasure", scientificAngleMeasureCombo->currentIndex());
	
	settings->Save("IntegerNumbers", "notation", integerNotationCombo->currentIndex());
	settings->Save("IntegerNumbers", "showNotation", showNotationCheck->checkState());

	settings->Save("RationalNumbers", "form", rationalFormCombo->currentIndex());
}

void MathResultPage::OnUpButtonClicked()
{
	int i = resultsOrderList->currentRow();
	if (i <= 0)
		return;
	QListWidgetItem* item = resultsOrderList->takeItem(i);
	resultsOrderList->insertItem(i - 1, item);
	resultsOrderList->setCurrentRow(i - 1);
}

void MathResultPage::OnDownButtonClicked()
{
	int i = resultsOrderList->currentRow();
	if (i == -1 || i == resultsOrderList->count() - 1)
		return;
	QListWidgetItem* item = resultsOrderList->takeItem(i);
	resultsOrderList->insertItem(i + 1, item);
	resultsOrderList->setCurrentRow(i + 1);
}
