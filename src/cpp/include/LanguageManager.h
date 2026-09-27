#pragma once

#include <QObject>
#include <QTranslator>

class AppSettings;
class QQmlApplicationEngine;

class LanguageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language NOTIFY languageChanged)

public:
    LanguageManager(AppSettings *settings, QQmlApplicationEngine *engine, QObject *parent = nullptr);

    QString language() const;

    Q_INVOKABLE void setLanguage(const QString &language);

signals:
    void languageChanged();

private:
    void applyLanguage();

    AppSettings *m_settings;
    QQmlApplicationEngine *m_engine;
    QTranslator m_translator;
    QString m_language;
    bool m_translatorInstalled = false;
};