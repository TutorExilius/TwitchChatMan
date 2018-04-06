#include "ircchat.h"

#include "config.h"
#include "chatmanager.h"
#include "login.h"

#include <QDebug>

IrcChat::IrcChat( QObject *parent )
: QObject{ parent }
, config{ nullptr }
, socket{ nullptr }
, chatManager{ dynamic_cast<ChatManager*>( parent ) }
, connectionData{ nullptr }
, currentChannel{ "TutorExilius" }
{
    this->config = new Config{
 R"(C:\Users\Tutor Exilius\Desktop\build-TwitchChatMan-Desktop_Qt_5_10_1_MinGW_32bit-Debug\debug\config.txt)", this };

    this->socket = new QTcpSocket{ this };

    if( this->chatManager == nullptr )
    {
        qDebug() << "Parent of IrcChat must be a pointer of ChatManager!";
        throw "Parent of IrcChat must be a pointer of ChatManager!";
    }

    // Todo: new and delete of that object in different classes!
    //       better: use smartpointer for autom. delete

    this->connectionData = const_cast<ConnectionData*>( this->config->getConnectionData() );
}

IrcChat::~IrcChat()
{
    this->disconnectFromServer();
    delete this->connectionData;
}

void IrcChat::connectToChannel( const QByteArray &channel )
{
    if( this->connectionData != nullptr )
    {
        this->currentChannel = channel;

        QObject::connect( this->socket, &QTcpSocket::readyRead,
                          this->chatManager, &ChatManager::readIrcChatData,
                          Qt::UniqueConnection );

        const QByteArray lowerNick = connectionData->nick.toLower();
        const QByteArray lowerChannel = channel.toLower();

        // Your nickname (NICK) must be your Twitch user name in lowercase.
        // see: https://dev.twitch.tv/docs/irc (Connecting to Twitch IRC)   [03.03.2018]

        // Twitch Irc Chat Host + Port
        socket->connectToHost( connectionData->host, connectionData->port );

        // Streamsr OAutch Key
        socket->write("PASS " + connectionData->pass + "\r\n");

        // Streamers Nick
        socket->write("NICK " + lowerNick + "\r\n");

        // Channel
        socket->write("JOIN #" + lowerChannel + "\r\n");

        // channel = nick!
        // example: streamername(nick): TutorExilius. Channel: #TutorExilius
    }
    else
    {
        qDebug() << "Maybe User or Pass wrong";
    }
}

 QVector<QString> IrcChat::getDataLines()
 {
     QVector<QString> chatLines;

     do
     {
        QString readLine = this->socket->readLine();
        chatLines.push_back( readLine );

     } while( socket->canReadLine() );

     return chatLines;
}

void IrcChat::disconnectFromServer()
{
    socket->write("PART #" + this->currentChannel + "\r\n"); // Good bye is optional message
    socket->flush();

    socket->disconnectFromHost();

   // QObject::disconnect( this->socket, &QTcpSocket::readyRead,
    //                     this, &IrcChat::readData );
    socket->disconnect(); 
    socket->close();
}
