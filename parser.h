#ifndef PARSER_H
#define PARSER_H

#include <QMap>
#include <QObject>
#include <QString>

// Forward-Declarations
class ChatManager;
class ChatMessage;

class Parser : public QObject
{
    Q_OBJECT

public:
    explicit Parser( QObject *parent );

    void reset();

    bool isGood() const
    {
        return this->good;
    }

    bool hasNewContend() const
    {
        if( this->parsedChatMessages->size() > 0 )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void parse( const QString &html, const uint &parseGreaterMessageId );
    const QMap<uint, ChatMessage>* getNewMessages() const;
    void deleteAllMessages();

signals:

private:
    Parser() = delete;
    Parser( const Parser &obj ) = delete;
    Parser& operator=( const Parser &obj ) = delete;

    QString extractName( const QString &chatMessageHtmlPart ) const;
    QString extractMessage( const QString &chatMessageHtmlPart ) const;

    bool good;
    ChatManager *chatManager;
    QMap<uint, ChatMessage> *parsedChatMessages;
};

#endif // PARSER_H
