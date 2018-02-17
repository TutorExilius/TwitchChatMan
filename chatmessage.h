#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QObject>
#include <QString>
#include <QDateTime>

class ChatMessage : public QObject
{
    Q_OBJECT

public:
    explicit ChatMessage( QObject *parent,
                 const qulonglong &id,
                 const QDateTime &dateTime,
                 const QString &user,
                 const QString &message );

    const qulonglong& getId() const
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
    ChatMessage() = delete;
    ChatMessage( const ChatMessage &obj ) = delete;
    ChatMessage& operator=( const ChatMessage &obj ) = delete;

    const qulonglong id;
    const QDateTime dateTime;
    const QString user;
    const QString message;
};

#endif // CHATMESSAGE_H
