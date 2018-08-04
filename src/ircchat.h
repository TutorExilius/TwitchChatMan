#ifndef IRCCHAT_H
#define IRCCHAT_H

#include <QObject>
#include <QByteArray>
#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

#include "connectiondata.h"

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
    void send( const QByteArray &message );
    void flush();

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
