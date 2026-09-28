#pragma once

#include <QObject>
#include <QProcess>

struct GameInfo {
    QString gamePath;
    QString gameCWD;
    QString gameName;
    QString gameIcon;
};

class GameLauncher : public QObject {
    Q_OBJECT
public:
    explicit GameLauncher(QObject *parent = nullptr);

    Q_INVOKABLE bool launchGame(const QString &gamePath, const QString &gameCWD, const QString &gameName, const QString &gameIcon, const bool &isPC98);
    Q_INVOKABLE bool launchWithThcrap(const QString &configPath, const QString &gamePath, const QString &gameCWD, const QString &gameName, const QString &gameIcon);
    Q_INVOKABLE bool launchWithThprac(const QString &gamePath, const QString &gameCWD, const QString &gameName, const QString &gameIcon);

    bool LaunchThread(const QString &gamePath, const QString &gameCWD, const QString &gameName, const QString &gameIcon);
    bool LaunchThcrapThread(const QString &configPath, const QString &gamePath, const QString &gameCWD, const QString &gameName, const QString &gameIcon);
    bool LaunchPC98Thread(const QString &gamePath, const QString &gameName, const QString &gameIcon);

    bool CheckGameRunning();

    GameInfo GetCurrentGameInfo();

signals:
    void launchError(const QString &errorCode);

private:
    const QString GetWinePathFromSettings();
    const QString GetDosboxXPathFromSettings();
    bool LaunchLinux(const QString &gamePath, const QString &gameCWD);
    bool Launch_PC98(const QString &gamePath);
    bool LaunchWindows(const QString &gamePath, const QString &gameCWD);

    QProcess *m_thpracProcess = nullptr;
};
