#include <QtGui>
#include <QFileDialog>
#include "SettingsDialogPages.h"
#include "../FormulaNodes/FormulaNode.h"

//SystemPage

/**
 * Constructor.
 * @param [in] _settings Program options.
 * @param [in] parent	The parent window.
 */
SystemPage::SystemPage(Settings* _settings, QWidget* parent) : QWidget(parent), settings(_settings)
{
	QVBoxLayout* mainLayout = new QVBoxLayout;

	startWithWindowsCheck = new QCheckBox(tr("Start the application when Windows starts"));
	startWithWindowsCheck->setChecked(settings->Load("System", "StartWithWindows", 0).toInt());
	mainLayout->addWidget(startWithWindowsCheck);
	
	trayIconCheck = new QCheckBox(tr("Tray icon"));
	trayIconCheck->setChecked(settings->Load("System", "TrayIcon", 1).toInt());
	mainLayout->addWidget(trayIconCheck);
	
	minimizeToTrayCheck = new QCheckBox(tr("Minimize to tray"));
	minimizeToTrayCheck->setChecked(settings->Load("System", "TrayIcon", 1).toInt());
	mainLayout->addWidget(minimizeToTrayCheck);

	documentsGroup = new QGroupBox(tr("Documents"));
	mainLayout->addWidget(documentsGroup);
	
	QVBoxLayout* documentsGroupLayout = new QVBoxLayout;
	
	openLastCheck = new QCheckBox(tr("Open the last document"));
	documentsGroupLayout->addWidget(openLastCheck);
	
	QHBoxLayout* hLayout = new QHBoxLayout;
	
	hLayout->addWidget(new QLabel(tr("Previously opened files list scope: ")));
	previousScopeSpin = new QSpinBox;
	previousScopeSpin->setMinimum(1);
	previousScopeSpin->setMaximum(10);
	hLayout->addWidget(previousScopeSpin);
	
	documentsGroupLayout->addLayout(hLayout);
	
	documentsGroupLayout->addWidget(new QLabel(tr("Documents folder:")));
	
	hLayout = new QHBoxLayout;
	
	documentsFolderLine = new QLineEdit;
	documentsFolderLine->setMinimumWidth(200);
	
#ifdef Q_WS_WIN
	QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
	settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
	documentsFolderLine->setText(settings.value("Personal").toString());
#endif

	hLayout->addWidget(documentsFolderLine);
	documentsFolderButton = new QPushButton(tr("..."));
	connect(documentsFolderButton, SIGNAL(clicked()), this, SLOT(OnDocumentsFolderButtonClicked()));
	documentsFolderButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	hLayout->addWidget(documentsFolderButton);
	hLayout->setSizeConstraint(QLayout::SetFixedSize);

	documentsGroupLayout->addLayout(hLayout);
	
	documentsGroup->setLayout(documentsGroupLayout);

	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(mainLayout);
}

/**
 * Stores the settings.
 */
void SystemPage::Store()
{
	settings->Save("System", "StartWithWindows", startWithWindowsCheck->checkState());
	settings->Save("System", "TrayIcon", trayIconCheck->checkState());
	settings->Save("System", "MinimizeToTray", minimizeToTrayCheck->checkState());
	
	settings->Save("System", "OpenLast", openLastCheck->checkState());
	settings->Save("System", "PreviousScope", previousScopeSpin->value());
	settings->Save("System", 	"DocumentsFolder", documentsFolderLine->text());
}

/**
 * Executes the documents folder button clicked action.
 */
void SystemPage::OnDocumentsFolderButtonClicked()
{
	QFileDialog dirDialog(this, tr("Choose a folder"), documentsFolderLine->text());
	dirDialog.setFileMode(QFileDialog::Directory);
	if (dirDialog.exec())
		documentsFolderLine->setText(dirDialog.selectedFiles()[0]);
}

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

//ColorsComboBox

/**
 * Constructor.
 * @param [in] parent	The parent widget.
 */
ColorsComboBox::ColorsComboBox(QWidget* parent) : QComboBox(parent)
{
	PopulateList();
}

/**
 * Returns the item's color
 */
QString ColorsComboBox::Color()
{
	return qVariantValue<QColor>(itemData(currentIndex(), Qt::UserRole)).name();
}

/**
 * Sets the item
 * @param [in] color The color
 */
void ColorsComboBox::SetColor(QString& color)
{
	setCurrentIndex(findData(color, int(Qt::UserRole)));
}

/**
 * Makes the combobox'es list
 */
void ColorsComboBox::PopulateList()
{
	QStringList colorNames = QColor::colorNames();
	int size = style()->pixelMetric(QStyle::PM_SmallIconSize);
	QPixmap pixmap(size, size);

	for (int i = 0; i < colorNames.size(); ++i)
	{
		QColor color(colorNames[i]);
		insertItem(i, colorNames[i]);
		pixmap.fill(color);
		setItemData(i, pixmap, Qt::DecorationRole);
		setItemData(i, color.name(), Qt::UserRole);
	}
}


//FormulaColorsPage

/**
 * Constructor.
 * @param [in] _settings The application settings.
 * @param [in] parent	The parent widget.
 */
FormulaColorsPage::FormulaColorsPage(Settings* _settings, QWidget* parent) : QWidget(parent), settings(_settings)
{
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->setAlignment(Qt::AlignTop);

	mainLayout->addWidget(new QLabel(tr("Numbers: ")), 0, 0);
	numbersColor = new ColorsComboBox;
	numbersColor->SetColor(settings->Load("Colors", "Numbers", QColor("blue").name()).toString());
	mainLayout->addWidget(numbersColor, 0, 1);

	mainLayout->addWidget(new QLabel(tr("Variables: ")), 1, 0);
	variablesColor = new ColorsComboBox;
	variablesColor->SetColor(settings->Load("Colors", "Variables", QColor("darkblue").name()).toString());
	mainLayout->addWidget(variablesColor, 1, 1);

	mainLayout->addWidget(new QLabel(tr("Functions: ")), 2, 0);
	functionsColor = new ColorsComboBox;
	functionsColor->SetColor(settings->Load("Colors", "Functions", QColor("orange").name()).toString());
	mainLayout->addWidget(functionsColor, 2, 1);

	mainLayout->addWidget(new QLabel(tr("Units: ")), 3, 0);
	unitsColor = new ColorsComboBox;
	unitsColor->SetColor(settings->Load("Colors", "Units", QColor("green").name()).toString());
	mainLayout->addWidget(unitsColor, 3, 1);

	mainLayout->addWidget(new QLabel(tr("Graphical elements: ")), 4, 0);
	graphicalElementsColor = new ColorsComboBox;
	graphicalElementsColor->SetColor(settings->Load("Colors", "GraphicalElements", QColor("black").name()).toString());
	mainLayout->addWidget(graphicalElementsColor, 4, 1);

	mainLayout->addWidget(new QLabel(tr("Error: ")), 5, 0);
	errorColor = new ColorsComboBox;
	errorColor->SetColor(settings->Load("Colors", "Error", QColor("red").name()).toString());
	mainLayout->addWidget(errorColor, 5, 1);

	mainLayout->addWidget(new QLabel(tr("Solving highlight: ")), 6, 0);
	solvingHighlightColor = new ColorsComboBox;
	solvingHighlightColor->SetColor(settings->Load("Colors", "SolvingHightlight", QColor("orange").name()).toString());
	mainLayout->addWidget(solvingHighlightColor, 6, 1);

	mainLayout->addWidget(new QLabel(tr("Text: ")), 7, 0);
	textColor = new ColorsComboBox;
	textColor->SetColor(settings->Load("Colors", "Text", QColor("grey").name()).toString());
	mainLayout->addWidget(textColor, 7, 1);

	mainLayout->addWidget(new QLabel(tr("Cursor: ")), 8, 0);
	cursorColor = new ColorsComboBox;
	cursorColor->SetColor(settings->Load("Colors", "Cursor", QColor("black").name()).toString());
	mainLayout->addWidget(cursorColor, 8, 1);

	mainLayout->addWidget(new QLabel(tr("Cursor highlight: ")), 9, 0);
	cursorHighlightColor = new ColorsComboBox;
	cursorHighlightColor->SetColor(settings->Load("Colors", "CursorHightlight", QColor("grey").name()).toString());
	mainLayout->addWidget(cursorHighlightColor, 9, 1);

	mainLayout->addWidget(new QLabel(tr("Background: ")), 10, 0);
	backgroundColor = new ColorsComboBox;
	backgroundColor->SetColor(settings->Load("Colors", "Background", QColor("white").name()).toString());
	mainLayout->addWidget(backgroundColor, 10, 1);

	mainLayout->setSizeConstraint(QLayout::SetFixedSize);

	setLayout(mainLayout);
}

/**
 * Stores data from this page to the settings.
 */
void FormulaColorsPage::Store()
{
	settings->Save("Colors", "Numbers", numbersColor->Color());
	settings->Save("Colors", "Variables", variablesColor->Color());
	settings->Save("Colors", "Functions", functionsColor->Color());
	settings->Save("Colors", "Units", unitsColor->Color());
	settings->Save("Colors", "GraphicalElements", graphicalElementsColor->Color());
	settings->Save("Colors", "Error", errorColor->Color());
	settings->Save("Colors", "SolvingHighlight", solvingHighlightColor->Color());
	settings->Save("Colors", "Text", textColor->Color());
	settings->Save("Colors", "Cursor", cursorColor->Color());
	settings->Save("Colors", "CursorHightlight", cursorHighlightColor->Color());
	settings->Save("Colors", "Background", backgroundColor->Color());
}

//MathResultPage

/**
 * Constructor.
 * @param [in] _settings The program settings.
 * @param [in] parent	The parent widget.
 */
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
			i->setCheckState((Qt::CheckState)settings->Load("ResultsOrder", "scientificResultState", 1).toInt());
			resultsOrderList->addItem(i);
		}
		else if (i == settings->Load("ResultsOrder", "integerResultPos", 1).toInt())
		{
			QListWidgetItem* i = new QListWidgetItem(tr("Integer"), resultsOrderList);
			i->setData(Qt::UserRole, INTEGER);
			i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
			i->setCheckState((Qt::CheckState)settings->Load("ResultsOrder", "integerResultState", 1).toInt());
			resultsOrderList->addItem(i);
		}
		else if (i == settings->Load("ResultsOrder", "rationalResultPos", 2).toInt())
		{
			QListWidgetItem* i = new QListWidgetItem(tr("Rational"), resultsOrderList);
			i->setData(Qt::UserRole, RATIONAL);
			i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
			i->setCheckState((Qt::CheckState)settings->Load("ResultsOrder", "rationalResultState", 1).toInt());
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

/**
 * Stores the settings.
 */
void MathResultPage::Store()
{
	for (int i = 0; i < resultsOrderList->count(); ++i)
	{
		QListWidgetItem* item = resultsOrderList->item(i);
		switch (item->data(Qt::UserRole).toInt())
		{
		case REAL:
			settings->Save("ResultsOrder", "scientificResultPos", i);
			settings->Save("ResultsOrder", "scientificResultState", item->checkState());
			break;
		case INTEGER:
			settings->Save("ResultsOrder", "integerResultPos", i);
			settings->Save("ResultsOrder", "integerResultState", item->checkState());
			break;
		case RATIONAL:
			settings->Save("ResultsOrder", "rationalResultPos", i);
			settings->Save("ResultsOrder", "rationalResultState", item->checkState());
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

/**
 * Executes the up button clicked action.
 */
void MathResultPage::OnUpButtonClicked()
{
	int i = resultsOrderList->currentRow();
	if (i <= 0)
		return;
	QListWidgetItem* item = resultsOrderList->takeItem(i);
	resultsOrderList->insertItem(i - 1, item);
	resultsOrderList->setCurrentRow(i - 1);
}

/**
 * Executes the down button clicked action.
 */
void MathResultPage::OnDownButtonClicked()
{
	int i = resultsOrderList->currentRow();
	if (i == -1 || i == resultsOrderList->count() - 1)
		return;
	QListWidgetItem* item = resultsOrderList->takeItem(i);
	resultsOrderList->insertItem(i + 1, item);
	resultsOrderList->setCurrentRow(i + 1);
}
