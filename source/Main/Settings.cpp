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
	
	SetValue("InterNodeSpace", NORMAL_LEVEL, 3);
	SetValue("InterNodeSpace", LESS_LEVEL, 2);
	SetValue("InterNodeSpace", STILL_LESS_LEVEL, 1);
	
	SetValue("InterSymbolSpace", NORMAL_LEVEL, 3);
	SetValue("InterSymbolSpace", LESS_LEVEL, 2);
	SetValue("InterSymbolSpace", STILL_LESS_LEVEL, 1);
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
	
	//call the slots being concerned with
	emit SettingsChanged(prefix, key);
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

/**
 * Sets a value.
 * @param name The name.
 * @param level The node level.
 * @param value The value.
 */
void Settings::SetValue(QString name, FormulaNodeLevel level, int value)
{
	values[name][level] = value;
}

/**
 * Gets a value.
 * @param name The name.
 * @param level The node level.
 * @return The value.
 */
int Settings::GetValue(QString name, FormulaNodeLevel level)
{
	//in proportion to the font size
	return values[name][level] * textFormulaNodeFonts[level].pointSize() / 10;
}
