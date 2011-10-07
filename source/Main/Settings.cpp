#include "Settings.h"
#include "../FormulaNodes/FormulaNode.h"
#include <QFont>

Settings::Settings(const QString& organization, const QString& application, QObject* parent) : 
	QSettings(QSettings::UserScope, organization, application, parent)
{
	qRegisterMetaType<FontMap>();
	qRegisterMetaTypeStreamOperators<FontMap>();
	
	textFormulaNodeFonts[NORMAL_LEVEL] = QFont("Times New Roman", 20);
	textFormulaNodeFonts[LESS_LEVEL] = QFont("Times New Roman", 16);
	textFormulaNodeFonts[STILL_LESS_LEVEL] = QFont("Times New Roman", 12);
	QVariant v;
	v.setValue(textFormulaNodeFonts);
	textFormulaNodeFonts = Load("Formulas", "textFormulaNodeFonts", v).value<FontMap>();
	
	setValue("InterSymbolSpace", 2);
}

Settings::~Settings()
{
	QVariant v;
	v.setValue(textFormulaNodeFonts);
	Save("Formulas", "textFormulaNodeFonts", v);
}

void Settings::Save(const QString& prefix, const QString& key, const QVariant& value)
{
	beginGroup(prefix);
	setValue(key, value);
	endGroup();
}

QVariant Settings::Load(const QString& prefix, const QString& key, const QVariant& defaultValue)
{
	beginGroup(prefix);
	QVariant res = value(key, defaultValue);
	endGroup();
	return res;
}

QFont& Settings::GetTextFormulaNodeFont(FormulaNodeLevel level)
{
	return textFormulaNodeFonts[level];
}
