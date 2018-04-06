#include "parser.h"

#include "chatmanager.h"
#include "chatmessage.h"

#include <QDebug>
#include <QException>
#include <QRegularExpression>

Parser::Parser( QObject *parent )
: QObject( parent )
, good{ true }
, chatManager{ dynamic_cast<ChatManager*>( parent ) }
, parsedChatMessages{ new QMap<uint, ChatMessage> }
{
    if( this->chatManager == nullptr )
    {
        qDebug() << "Parent of Parser must be a pointer of ChatManager!";
        throw "Parent of Parser must be a pointer of ChatManager!";
    }
}

void Parser::reset()
{
    this->good = true;
    this->parsedChatMessages->clear();
}

// TODO: optimize this method! -> delegate matching-steps and delegate ChatMessage-Construction
//       (reduce if-construct depth)
void Parser::parse( const QString &html, const uint &parseGreaterMessageId )
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

                                    /*Map*/
                                        this->parsedChatMessages->insert(
                                        /*key*/     id,
                                        /*value*/   ChatMessage{ id,
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

const QMap<uint, ChatMessage>* Parser::getNewMessages() const
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


QString Parser::extractName( const QString &chatMessageHtmlPart ) const
{
    QRegularExpression reg{ R"(class="from".*?>.*?</span>)",
                            QRegularExpression::DotMatchesEverythingOption };

    QRegularExpressionMatchIterator it = reg.globalMatch( chatMessageHtmlPart );

    if( it.hasNext() )
    {
        const QRegularExpressionMatch match = it.next();
        QString name = match.captured(0);

        int startPos = name.indexOf(">");
        int endPos = name.indexOf("<");

        if( startPos >= 0 && endPos >= 0 )
        {
            name = name.mid( startPos + 1, (endPos - startPos) - 1 );
        }

        return name;
    }
    else
    {
        return "ErrorName";
    }
}

QString Parser::extractMessage( const QString &chatMessageHtmlPart ) const
{
    QRegularExpression reg{ R"(class="message">.*?</span>.*?<!)",
                            QRegularExpression::DotMatchesEverythingOption };

    QRegularExpressionMatchIterator it = reg.globalMatch( chatMessageHtmlPart );

    if( it.hasNext() )
    {
        const QRegularExpressionMatch match = it.next();
        QString stringPart = match.captured(0);

        int spanStart = stringPart.indexOf( "<span" );
        int spanEnd = stringPart.indexOf( "</span>" );
        const int lenOfSpanEnd = 7;
        while( spanStart >= 0 && spanEnd >= 0 )
        {
            stringPart = stringPart.replace( spanStart,
                         spanEnd-spanStart+lenOfSpanEnd,
                         "" );

            spanStart = stringPart.indexOf( "<span" );
            spanEnd = stringPart.indexOf( "</span>" );
        }

        QRegularExpression regPart{ R"(">.*?</span>)",
                                    QRegularExpression::DotMatchesEverythingOption };

        QRegularExpressionMatchIterator it_part = regPart.globalMatch( stringPart );

        if( it_part.hasNext() )
        {
            const QRegularExpressionMatch match_part = it_part.next();
            QString messagePart = match_part.captured(0);
            messagePart = messagePart.remove( 0, 2 );
            messagePart = messagePart.remove( messagePart.size()-8, 8 );

            // Todo:: extract all urls
            if( messagePart.indexOf( "<a" ) >= 0 )
            {

            }

            return messagePart.trimmed();
        }
        else
        {
            return stringPart;
        }
    }
    else
    {
        return "";
    }
}
