#ifndef IRCCHAT_H
#define IRCCHAT_H

#include <QObject>
#include <QByteArray>
#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

struct ConnectionData
{
    ConnectionData( const QByteArray nick,
                    const QByteArray pass,
                    const QString &host = "irc.chat.twitch.tv",
                    const quint16 &port = 6667 )
    : nick{ nick }
    , pass{ pass }
    , host{ host }
    , port{ port }
    {
    }

    const QString host;
    const QByteArray nick;
    const QByteArray pass;
    const quint16 port;
};

// Forward-Declarations
class Config;
class ChatManager;

class IrcChat : public QObject
{
    Q_OBJECT

public:
    static ConnectionData* getConnectionData();

    explicit IrcChat( QObject *parent );
    ~IrcChat();

    void connectToChannel( const QByteArray &channel );

    QVector<QString> getDataLines();

private slots:
    void disconnectFromServer();

private:
    Config *config;
    QTcpSocket *socket;
    ChatManager *chatManager;
    ConnectionData *connectionData;
    QByteArray currentChannel;
};

#endif // IRCCHAT_H
