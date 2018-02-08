#include "chatparser.h"

#include <QDebug>
#include <QVector>

ChatParser::ChatParser( QObject *parent )
: QObject{ parent }
, ok{ true }
, lastMessagedId{ 0 }
, parsedMessages{ new QVector<ChatMessage*> }
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

        // TODO: parse html from last ID to end of html file

        this->ok = true;
    }
    else
    {
        this->ok = false;
        qDebug() << "pop parsed messages first before next parse-cycle!";
    }
}

QVector<ChatMessage*>* ChatParser::popParsedChatMessages()
{
    QVector<ChatMessage*>* tmp{ this->parsedMessages };

    this->parsedMessages->resize( 0 );

    return tmp;
}
