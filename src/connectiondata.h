#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

#include <QByteArray>
#include <QString>

struct ConnectionData
{
    ConnectionData( const QByteArray nick,
                    const QByteArray pass,
                    const QString &host = "irc.chat.twitch.tv",
                    const quint16 &port = 6667,
                    const QString &joinedChannel = "TutorExilius" )
    : nick{ nick }
    , pass{ pass }
    , host{ host }
    , port{ port }
    , joinedChannel{ joinedChannel }
    {
    }

    const QByteArray nick;
    const QByteArray pass;
    const QString host;
    const quint16 port;
    QString joinedChannel;
};
#endif // CONNECTIONDATA_H
