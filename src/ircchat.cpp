#include "ircchat.h"

#include "config.h"
#include "chatmanager.h"

#include <QDateTime>
#include <QDebug>
#include <QTimer>

IrcChat::IrcChat( QObject *parent )
: QObject{ parent }
, config{ nullptr }
, socket{ nullptr }
, chatManager{ dynamic_cast<ChatManager*>( parent ) }
, connectionData{ nullptr }
, currentChannel{ "TutorExilius" }
, chatBuffer{ nullptr }
, timer{ this }
{
    this->config = new Config{"config.txt", this };
    this->socket = new QTcpSocket{ this };
    this->chatBuffer = new QList<QString>;

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


        const QByteArray lowerNick = connectionData->nick.toLower();
        const QByteArray lowerChannel = channel.toLower();

        // Your nickname (NICK) must be your Twitch user name in lowercase.
        // see: https://dev.twitch.tv/docs/irc (Connecting to Twitch IRC)   [03.03.2018]

        // Twitch Irc Chat Host + Port
        socket->connectToHost( connectionData->host, connectionData->port );

        // Anweisung an den IRC Server, badges mitzusenden (twitch format)
        //socket->write( "CAP REQ :twitch.tv/membership\r\nCAP REQ :twitch.tv/tags\r\nCAP REQ :twitch.tv/commands\r\n");

        this->blockSignals(true);
        // Streamers OAutch Key
        socket->write("PASS " + connectionData->pass + "\r\n");

        // Streamers Nick
        socket->write("NICK " + lowerNick + "\r\n");

        // Channel
        socket->write("JOIN #" + lowerChannel + "\r\n");

        QObject::connect( this->socket, &QTcpSocket::readyRead,
                          this, &IrcChat::timerStart,
                          Qt::UniqueConnection );
        /*
        QObject::connect( this->socket, &QTcpSocket::readyRead,
                              this->chatManager, &ChatManager::readIrcChatData,
                              Qt::UniqueConnection );
                              */
    }
    else
    {
        qDebug() << "Maybe User or Pass wrong";
    }
}

void IrcChat::send( const QByteArray &message )
{
    qDebug() << "SEND to server: " << message;
    qDebug() << "\t" << "Succesfully send " << this->socket->write( message ) << " Bytes";
}

void IrcChat::flush()
{
     this->socket->flush();
}

QVector<QString> IrcChat::getDataLines()
{
     QVector<QString> chatLines;

     do
     {
        QString readLine = this->socket->readLine();

        // Answer to PING
        if( readLine == "PING :tmi.twitch.tv\r\n" )
        {
            this->send( "PONG :tmi.twitch.tv\r\n" );
            this->flush();
        }

        chatLines.push_back( readLine );

     } while( socket->canReadLine() );

     return chatLines;
}

void IrcChat::timerStart()
{
    qDebug() << "Timer started";

    QObject::disconnect( this->socket, &QTcpSocket::readyRead,
                  this, &IrcChat::timerStart );


    QObject::connect( &this->timer, &QTimer::timeout,
                     this, &IrcChat::handleChannelJoin,
                     Qt::UniqueConnection );

    this->timer.start( 3000 );
}

void IrcChat::handleChannelJoin()
{
    qDebug() << "Timer stopped (timeout)";
    this->timer.stop();

    if( this->successfullyJoined() )
    {
        QObject::connect( this->socket, &QTcpSocket::readyRead,
                         this->chatManager, &ChatManager::readIrcChatData,
                         Qt::UniqueConnection );

        this->chatManager->addChatMessageToList( "Successfully Joined" );
    }
    else
    {
        this->chatManager->addChatMessageToList( "Channel Join failed. User exists?" );
    }
}

bool IrcChat::successfullyJoined()
{
    do
    {
       QString readLine = this->socket->readLine();
qDebug() << readLine;
       if( readLine.contains(":End of /NAMES list\r\n") )
       {
           return true;
       }

    } while( socket->canReadLine() );

    return false;
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
