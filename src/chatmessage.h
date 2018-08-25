#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>
#include <QTextStream>
#include <QDateTime>

class ChatMessage
{
public:
    ChatMessage();
    explicit ChatMessage( qint64 id,
                          const QDateTime &dateTime,
                          const QString &user,
                          const QString &message,
                          const bool fromArchive = false );

    ChatMessage( const ChatMessage &obj );
    ChatMessage& operator=( const ChatMessage &obj );

    qint64  getId() const
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

    bool isFromArchive() const
    {
        return this->fromArchive;
    }

    const QString saveFormat() const
    {
        QString out;

        out += QString::number( this->id ) + "\n";
        out += this->dateTime.toString() + "\n";
        out += this->user + "\n";
        out += this->message + "\n";

        return out;
    }

private:
    qint64  id;
    QDateTime dateTime;
    QString user;
    QString message;
    bool fromArchive;
};

#endif // CHATMESSAGE_H
