#include "chatparser.h"

#include <QDebug>

#include <QRegularExpression>

ChatParser::ChatParser( QObject *parent )
: QObject{ parent }
, ok{ true }
, lastMessagedId{ 0 }
, parsedMessages{ new QMap<qulonglong,ChatMessage*> }
, messages{ new QList<QString*> }
{
}

bool ChatParser::isOk() const
{
   return this->ok;
}

void ChatParser::parse( const QString &html )
{
    if( this->parsedMessages->size() == 0 )
    {
        //QList<QString*> *messages = new QList<QString*>;

    //   int pos = html.indexOf( R"(<div class=\"chat-line__message\")" );

        QRegularExpression reg{ "<div class=\"chat-line__message\""
                              ".*?<!-- \\/react-text -->", QRegularExpression::DotMatchesEverythingOption };
                                      // "(?=<div class="chat-line__message"))" };

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
                this->addChatMessage( chatMessageHtmlPart );
            }

            ++matchCounter;
        }

     /*   if( messages->size() != matchCounter )
        {
            qDebug() << "matches counter is different to count of parsed words!\tmatches: " << matchCounter
                     << "\t" << "parsed chats count: " << messages->size();
        }*/

        qDebug() << "matches: " << matchCounter;
        qDebug() << "Entries: " << this->messages->size();

        this->ok = true;
    }
    else
    {
        this->ok = false;
        qDebug() << "pop parsed messages first before next parse-cycle!";
    }
}

void ChatParser::addChatMessage( const QString &chatMessageHtmlPart )
{
    // TODO: LESS THAN 20% free space? -> write old chatmessages
    // into txtfile (archive) and delete from memory (list)!

    this->messages->append( new QString{chatMessageHtmlPart} );
}

QVector<ChatMessage*>* ChatParser::popParsedChatMessages()
{
    QVector<ChatMessage*>* tmp = nullptr;//{ this->parsedMessages };

    this->parsedMessages->clear();

    return tmp;
}
