#include "chatmanager.h"

#include "chatmessage.h"
#include "mainwindow.h"
#include "ircchat.h"
#include "parser.h"

#include <QDateTime>
#include <QDebug>
#include <QException>
#include <QMap>
#include <QString>

ChatManager::ChatManager( QObject *parent )
: QObject{ parent }
, mainWindow{ dynamic_cast<MainWindow*>( parent ) }
, ircChat{ nullptr }
, parser{ nullptr }
, lastMessagedId{ 0 }
, chatMessages{ new QMap<uint,ChatMessage> }
{
    if( this->mainWindow == nullptr )
    {
        qDebug() << "Parent of ChatManager must be a pointer of MainWindow!";
        throw "Parent of ChatManager must be a pointer of MainWindow!";
    }

    this->parser = new Parser{ this };
}

ChatManager::~ChatManager()
{
}

ChatMessage ChatManager::getChatMessage( const uint &messageId ) const
{
    return this->chatMessages->value(messageId);
}

/*
bool ChatManager::parse( const QString &html )
{
    bool ret = false;

    try
    {
        if( html.size() > 0 )
        {
            this->parser->parse( html, this->lastMessagedId );

            if( this->parser->isGood() )
            {
                if( this->parser->hasNewContend() )
                {
                    ret = this->collectNewMessages( this->parser );
                }
            }
            else
            {
                qDebug() << "Error: ChatManager::parse() could not parse. Instance: Parser isOk() is false.";
            }
        }
        else
        {
            qDebug() << "html-size is NULL. Nothing to parse!";
        }
    }
    catch( QException ex )
    {
        qDebug() << "In 'ChatManager::parse( const QString &html )'\n\t" << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << "In 'ChatManager::parse( const QString &html )'\n\t" << ex;
    }
    catch( ... )
    {
        qDebug() << "In 'ChatManager::parse( const QString &html )'\n\t" << "CRASHED";
    }

    return ret;
}
        */

void ChatManager::readIrcChatData()
{
    // -- Auskommentiert, da wir erstmal das Programm ohne Zeitpuffer testen wollen

    //static QDateTime lastDateTime = QDateTime::currentDateTime();
    //const QDateTime currentDateTime = QDateTime::currentDateTime();

    //qint64 milliSecDiff = lastDateTime.msecsTo( currentDateTime );

    // if( milliSecDiff > 1000 )
    {
        QVector<QString> newChatLines = this->ircChat->getDataLines();

        QVector<ChatMessage> *newChatMessages = new QVector<ChatMessage>;

        for( const auto &line : newChatLines )
        {
            ChatMessage chatMessage = this->parser->parse( line );
            newChatMessages->push_back( chatMessage );

            if( !this->chatMessages->contains( chatMessage.getId()))
                this->chatMessages->insert( chatMessage.getId(), chatMessage );

            if( line == "PING :tmi.twitch.tv\r\n" )
            {
                this->ircChat->send( "PONG :tmi.twitch.tv\r\n" );
                this->ircChat->flush();
            }
        }


        this->mainWindow->fillChatMessageListView(newChatMessages);
        //lastDateTime = currentDateTime;
    }
}

void ChatManager::start( const QByteArray &joiningChannel )
{
    if( this->ircChat == nullptr )
    {
        this->ircChat = new IrcChat{ this };
    }

    this->ircChat->connectToChannel( joiningChannel );
}

/*
bool ChatManager::collectNewMessages( Parser *parser )
{
    bool ret = false;

    try
    {
        QVector<ChatMessage> *newChatMessages = new QVector<ChatMessage>;

        const QMap<uint, ChatMessage> *parsedChatMessages = this->parser->getNewMessages();

        int addedCount = 0;

        for( auto it = parsedChatMessages->begin(); it != parsedChatMessages->end(); it++ )
        {
            if( !this->chatMessages->contains( it.key() ) )
            {
                ++addedCount;
                newChatMessages->push_back( it.value() );

                this->chatMessages->insert( it.key(), it.value() );
            }
        }

        // save last id
        if( this->chatMessages->size() > 0 )
        {
            this->lastMessagedId = ( this->chatMessages->begin() + (this->chatMessages->size()-1) ).key();
        }

        if( addedCount > 0 )
        {
            qDebug() << "Added new messages:\t" << addedCount;
            qDebug() << "Count of all messages:\t" << this->chatMessages->size();

            this->parser->deleteAllMessages();

            this->mainWindow->updateChatMessageListView( newChatMessages );

            ret = true;
        }
    }
    catch( QException ex )
    {
        qDebug() << "In 'ChatManager::collectNewMessages()'\n\t" << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << "In 'ChatManager::collectNewMessages()'\n\t" << ex;
    }
    catch( ... )
    {
        qDebug() << "In 'ChatManager::collectNewMessages()'\n\t" << "CRASHED";
    }

    return ret;
}
*/

void ChatManager::reset()
{
    this->deleteAllChatMessages();
    this->resetParser();
}

void ChatManager::deleteAllChatMessages()
{
    this->chatMessages->clear();
    this->lastMessagedId = 0;
}

void ChatManager::resetParser()
{
    this->parser->reset();
}
