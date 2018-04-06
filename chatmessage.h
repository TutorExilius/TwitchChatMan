#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>
#include <QDateTime>

class ChatMessage
{
public:
    ChatMessage();
    explicit ChatMessage( const uint &id,
                          const QDateTime &dateTime,
                          const QString &user,
                          const QString &message );

    ChatMessage( const ChatMessage &obj );
    ChatMessage& operator=( const ChatMessage &obj );

    const uint& getId() const
    {
        return this->id;
    }

    const QString& getUser() const
    {
        return this->user;
    }

    const QDateTime& getDateTime() const
    {
        return this->dateTime;
    }

    const QString& getMessage() const
    {
        return this->message;
    }

private:
    uint id;
    QDateTime dateTime;
    QString user;
    QString message;
};

#endif // CHATMESSAGE_H
