#include "Settings.h"
#include "../FormulaNodes/FormulaNode.h"
#include <QFont>

/**
 * Constructor.
 * @param organization The organization name.
 * @param application The application name.
 * @param [in,out] parent The parent object.
 */
Settings::Settings(const QString& organization, const QString& application, QObject* parent) : 
	QSettings(QSettings::UserScope, organization, application, parent)
{
	qRegisterMetaType<FontMap>();
	qRegisterMetaTypeStreamOperators<FontMap>();
	
	//the text fonts
	textFormulaNodeFonts[NORMAL_LEVEL] = QFont("Times New Roman", 14);
	textFormulaNodeFonts[LESS_LEVEL] = QFont("Times New Roman", 12);
	textFormulaNodeFonts[STILL_LESS_LEVEL] = QFont("Times New Roman", 10);
	QVariant v;
	v.setValue(textFormulaNodeFonts);
	textFormulaNodeFonts = Load("Formulas", "textFormulaNodeFonts", v).value<FontMap>();
	
	setValue("InterNodeSpace", 5);
	setValue("InterSymbolSpace", 2);
}

/**
 * Destructor.
 */
Settings::~Settings()
{
	QVariant v;
	v.setValue(textFormulaNodeFonts);
	Save("Formulas", "textFormulaNodeFonts", v);
}

/**
 * Saves the settings.
 * @param prefix The prefix in a registry.
 * @param key	The key in a registry.
 * @param value The value.
 */
void Settings::Save(const QString& prefix, const QString& key, const QVariant& value)
{
	beginGroup(prefix);
	setValue(key, value);
	endGroup();
}

/**
 * Loads the settings.
 * @param prefix The prefix in a registry.
 * @param key	The key in a registry.
 * @param defaultValue The default value.
 * @return Loaded value.
 */
QVariant Settings::Load(const QString& prefix, const QString& key, const QVariant& defaultValue)
{
	beginGroup(prefix);
	QVariant res = value(key, defaultValue);
	endGroup();
	return res;
}

/**
 * Gets a text node font.
 * @param level The node level.
 * @return The text node font.
 */
QFont& Settings::GetTextFormulaNodeFont(FormulaNodeLevel level)
{
	return textFormulaNodeFonts[level];
}
