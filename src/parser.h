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

    /*
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
*/

    ChatMessage parse( const QString &line ) const;

    //void parse( const QString &html, const uint &parseGreaterMessageId );
   // const QMap<uint, ChatMessage>* getNewMessages() const;
   // void deleteAllMessages();

signals:

private:
    static uint messageCount;

    Parser() = delete;
    Parser( const Parser &obj ) = delete;
    Parser& operator=( const Parser &obj ) = delete;

    QString extractName( const QString &line ) const;
    QString extractMessage( const QString &line ) const;

    bool good;
 //   ChatManager *chatManager;
 //   QMap<uint, ChatMessage> *parsedChatMessages;
};

#endif // PARSER_H
