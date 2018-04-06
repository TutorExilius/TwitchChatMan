#include "chatmessage.h"

ChatMessage::ChatMessage()
: id{ 0 }
{
}

ChatMessage::ChatMessage( const uint &id,
                          const QDateTime &dateTime,
                          const QString &user,
                          const QString &message )
: id{ id }
, dateTime{ dateTime }
, user{ user }
, message{ message }
{
}

ChatMessage::ChatMessage( const ChatMessage &obj )
: id{ obj.id }
, dateTime{ obj.dateTime }
, user{ obj.user }
, message{ obj.message }
{
}

ChatMessage& ChatMessage::operator=( const ChatMessage &obj )
{
    this->id = obj.id;
    this->dateTime = obj.dateTime;
    this->user = obj.user;
    this->message = obj.message;

    return *this;
}
