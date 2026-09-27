#include "LanguageManager.h"

#include "AppSettings.h"

#include <QCoreApplication>
#include <QLocale>
#include <QQmlApplicationEngine>

LanguageManager::LanguageManager(AppSettings *settings, QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent), m_settings(settings), m_engine(engine)
{
    const QString savedLanguage = m_settings->value("language").toString();
    if (savedLanguage == "ja_JP" || savedLanguage == "en_US") {
        m_language = savedLanguage;
    } else {
        m_language = QLocale::system().language() == QLocale::Japanese ? "ja_JP" : "en_US";
    }

    applyLanguage();
}

QString LanguageManager::language() const
{
    return m_language;
}

void LanguageManager::setLanguage(const QString &language)
{
    if ((language != "ja_JP" && language != "en_US") || language == m_language) {
        return;
    }

    m_language = language;
    m_settings->setValue("language", m_language);
    applyLanguage();
    emit languageChanged();
}

void LanguageManager::applyLanguage()
{
    if (m_translatorInstalled) {
        QCoreApplication::instance()->removeTranslator(&m_translator);
        m_translatorInstalled = false;
    }

    if (m_language == "ja_JP") {
        if (m_translator.load(":/i18n/9Launcher_ja_JP.qm")) {
            QCoreApplication::instance()->installTranslator(&m_translator);
            m_translatorInstalled = true;
        } else {
            qWarning() << "Japanese translation could not be loaded; using English UI.";
        }
    }

    m_engine->retranslate();
}