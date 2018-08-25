#include "parser.h"

#include "chatmanager.h"
#include "chatmessage.h"

#include <QDebug>
#include <QException>
#include <QRegularExpression>

uint Parser::messageCount = 0;

Parser::Parser( QObject *parent )
: QObject( parent )
, good{ true }
//, chatManager{ dynamic_cast<ChatManager*>( parent ) }
//, parsedChatMessages{ new QMap<qint64 , ChatMessage> }
{
    /*if( this->chatManager == nullptr )
    {
        qDebug() << "Parent of Parser must be a pointer of ChatManager!";
        throw "Parent of Parser must be a pointer of ChatManager!";
    }
    */
}

void Parser::reset()
{
    this->good = true;
//    this->parsedChatMessages->clear();
}

ChatMessage Parser::parse( const QString &line ) const
{
    const QString username = this->extractName( line );
    const QString message = this->extractMessage( line );

    ++messageCount;

    return ChatMessage{ QDateTime::currentDateTime().toMSecsSinceEpoch(),
                        QDateTime::currentDateTime(),
                        username,
                        message };
}


/*
// TODO: optimize this method! -> delegate matching-steps and delegate ChatMessage-Construction
//       (reduce if-construct depth)
void Parser::parse( const QString &html, qint64 parseGreaterMessageId )
{
    this->reset(); // reset Parser (set good and emtpy parsedChatMesssages)

    if( html.size() > 0 )
    {
        try
        {
            QRegularExpression reg{ R"(<li id="ember\d+" class="message-line chat-line.*?</li>)",
                                    QRegularExpression::DotMatchesEverythingOption };

            QRegularExpressionMatchIterator it = reg.globalMatch( html );

            int matchCounter = 0;
            QString chatMessageHtmlPart;
            QRegularExpressionMatch match;

            while( it.hasNext() )
            {
                match = it.next();
                chatMessageHtmlPart = match.captured(0);

                if( chatMessageHtmlPart.size() > 0 )
                {
                    // Match MessageID-Tag "emberXXX" via RegEx
                    QRegularExpression emberIdReg{ R"(ember\d+)" };
                    QRegularExpressionMatchIterator it_ember = emberIdReg.globalMatch( chatMessageHtmlPart );
                    QRegularExpressionMatch matchEmber;

                    QString emberIdStr;

                    if( it_ember.hasNext() ) // MessageID-Tag "emberXXX" found
                    {
                         matchEmber = it_ember.next();
                         emberIdStr = matchEmber.captured(0);

                         if( emberIdStr.size() > 0 )
                         {
                             // Match ID
                             emberIdReg.setPattern( R"(\d+)" );
                             it_ember = emberIdReg.globalMatch( emberIdStr );

                             QString emberId;
                             if( it_ember.hasNext() ) // ID found
                             {
                                matchEmber = it_ember.next();
                                emberId = matchEmber.captured(0);

                                if( emberId.size() > 0 )
                                {
                                    // cast ID to int, is this new ID is greater (newer)
                                    // than last addes MessageID, add it to list
                                    const uint id = emberId.toULongLong();

                                    if( id > parseGreaterMessageId )
                                    {
                                        // construct ChatMessage and add into list


                                        this->parsedChatMessages->insert(
                                            id,
                                         ChatMessage{ id,
                                                                 QDateTime::currentDateTime(),
                                                                 this->extractName(chatMessageHtmlPart),
                                                                 this->extractMessage(chatMessageHtmlPart)
                                                                }
                                                    );

                                    }
                                }
                             }
                         }
                    }

                    ++matchCounter;
                }
            }

            qDebug() << "matches: " << matchCounter;

            this->good = true;
        }
        catch( QException ex )
        {
            this->good = false;
            qDebug() << "In 'Parser::parse( const QString &html, const uint &parseGreaterMessageId )'\n\t" << ex.what();
            
            this->reset();
        }
        catch( const char *ex )
        {
            this->good = false;
            qDebug() << "In 'Parser::parse( const QString &html, const uint &parseGreaterMessageId )'\n\t" << ex;
            
            this->reset();
        }
        catch( ... )
        {
            this->good = false;
            qDebug() << "In 'Parser::parse( const QString &html, const uint &parseGreaterMessageId )'\n\t" << "CRASHED";
            
            this->reset();
        }
        
        
    }
}

const QMap<qint64 , ChatMessage>* Parser::getNewMessages() const
{
    return this->parsedChatMessages;
}

void Parser::deleteAllMessages()
{
    try
    {
        this->parsedChatMessages->clear();
    }
    catch( QException ex )
    {
        qDebug() << "In 'Parser::deleteAllMessages()'\n\t" << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << "In 'Parser::deleteAllMessages()'\n\t" << ex;
    }
    catch( ... )
    {
        qDebug() << "In 'Parser::deleteAllMessages()'\n\t" << "CRASHED";
    }
}

*/

QString Parser::extractName( const QString &line ) const
{
    qDebug() << line ;
    QRegularExpression reg{ R"(^:[^\s]*!)",
                            QRegularExpression::DotMatchesEverythingOption };

    QRegularExpressionMatchIterator it = reg.globalMatch( line );

    if( it.hasNext() )
    {
        const QRegularExpressionMatch match = it.next();
        QString username = match.captured(0);
        username.remove( 0, 1 );
        username.remove( username.size() - 1, 1 );

        qDebug() << "Username: " << username;

        return username;
    }
    else
    {
        return "ERROR_USERNAME";
    }
}

QString Parser::extractMessage( const QString &line ) const
{
    QRegularExpression reg{ R"(^:[^\s]*?!.*?PRIVMSG #.*? :)",
                            QRegularExpression::DotMatchesEverythingOption };

    QString message = line;
    message.remove( reg );
    message.remove( "\r\n" );

    qDebug() << "Message: " << message;
    return message;
}
