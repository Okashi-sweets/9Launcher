#include <QObject>
#include <QSettings>
#include <QUrl>
#include <QVariant>
#include <QMetaType>
#include <qobject.h>

class AppSettings : public QSettings {
    Q_OBJECT

public:
    AppSettings(const QString &organization, const QString &application) : QSettings(organization, application) {}

    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue) const;

    Q_INVOKABLE QVariant value(const QString &key) const;

    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);

    Q_INVOKABLE bool setUrlValue(const QString &key, const QUrl &value);

    Q_INVOKABLE void clear();
};
