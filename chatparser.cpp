#include "chatparser.h"

#include <QDebug>
#include <QVector>

#include <QThread>


ChatParser::ChatParser( QObject *parent )
: QObject{ parent }
, ok{ true }
, lastMessagedId{ 0 }
, parsedMessages{ new QMap<qulonglong,ChatMessage*> }
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

        QString copy = html;

        const QString startString = R"(<div class="chat-line__message" data-a-target="chat-line-message">)";
        const QString endString = R"(<!-- /react-text -->)";

        bool containsMessage = true;

        QVector<QString> messages;

        int startPos = -1;
        int endPos= -1;

        do
        {
             startPos = copy.indexOf( startString );
             endPos = copy.indexOf( endString );

             if( startPos >= 0 && endPos > 0 )
             {
                 const QString messageHtmlPart =
                        copy.mid( startPos, copy.size() - (copy.size()-endPos) - startPos );

                 messages.push_back( messageHtmlPart );

                 copy = copy.mid( endPos + endString.size() );
                 qDebug() << "COPY: " << copy.size();
             }
             else
             {
                 containsMessage = false;
             }

        } while( containsMessage );

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
    QVector<ChatMessage*>* tmp = nullptr;//{ this->parsedMessages };

    this->parsedMessages->clear();

    return tmp;
}
