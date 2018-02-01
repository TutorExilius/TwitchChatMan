#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>
#include <QDateTime>

class ChatMessage
{
public:
    ChatMessage( const size_t &id,
                 const QDateTime &dateTime,
                 const QString &user,
                 const QString &message );

    size_t getId() const
    {
        return this->id;
    }

    QString getUser() const
    {
        return this->user;
    }

    QDateTime getDateTime() const
    {
        return this->dateTime;
    }

    QString getMessage() const
    {
        return this->message;
    }

private:
    // deletes
    ChatMessage& operator=( ChatMessage &obj ) = delete;

    const size_t id;
    const QDateTime dateTime;
    const QString user;
    const QString message;
};

#endif // CHATMESSAGE_H
