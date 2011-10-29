#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
//#include "../FormulaNodes/FormulaNode.h"

enum FormulaNodeLevel;

typedef QMap<int, QFont> FontMap;

Q_DECLARE_METATYPE(FontMap);

/**
 * The application settings.
 */
class Settings : public QSettings
{
public:
	Settings(const QString& organization, const QString& application = QString(), QObject* parent = 0);
	~Settings();
	
public:
	void Save(const QString& prefix, const QString& key, const QVariant& value);
	QVariant Load(const QString& prefix, const QString& key, const QVariant& defaultValue = QVariant());
	
	QFont& GetTextFormulaNodeFont(FormulaNodeLevel level);
	
private:
	FontMap textFormulaNodeFonts;
};

#endif
