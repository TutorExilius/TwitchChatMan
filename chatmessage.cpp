#include "chatmessage.h"

ChatMessage::ChatMessage( const size_t &id,
            const QDateTime &dateTime,
            const QString &user,
            const QString &message )
: id{ id }
, dateTime{ dateTime }
, user{ user }
, message{ message }
{
}
