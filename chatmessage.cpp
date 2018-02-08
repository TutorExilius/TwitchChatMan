#include "chatmessage.h"

ChatMessage::ChatMessage( QObject *parent,
            const qulonglong &id,
            const QDateTime &dateTime,
            const QString &user,
            const QString &message )
: QObject{ parent }
, id{ id }
, dateTime{ dateTime }
, user{ user }
, message{ message }
{
}
