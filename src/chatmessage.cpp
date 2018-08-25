#include "chatmessage.h"

ChatMessage::ChatMessage()
: id{ 0 }
, fromArchive{ false }
{
}

ChatMessage::ChatMessage( qint64 id,
                          const QDateTime &dateTime,
                          const QString &user,
                          const QString &message,
                          const bool fromArchive )
: id{ id }
, dateTime{ dateTime }
, user{ user }
, message{ message }
, fromArchive{ fromArchive }
{
}

ChatMessage::ChatMessage( const ChatMessage &obj )
: id{ obj.id }
, dateTime{ obj.dateTime }
, user{ obj.user }
, message{ obj.message }
, fromArchive{ obj.fromArchive }
{
}

ChatMessage& ChatMessage::operator=( const ChatMessage &obj )
{
    this->id = obj.id;
    this->dateTime = obj.dateTime;
    this->user = obj.user;
    this->message = obj.message;
    this->fromArchive = obj.fromArchive;

    return *this;
}
