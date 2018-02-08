#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QObject>
#include <QString>
#include <QDateTime>

class ChatMessage : public QObject
{
    Q_OBJECT

public:
    ChatMessage( QObject *parent,
                 const qulonglong &id,
                 const QDateTime &dateTime,
                 const QString &user,
                 const QString &message );

    qulonglong getId() const
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
    const qulonglong id;
    const QDateTime dateTime;
    const QString user;
    const QString message;
};

#endif // CHATMESSAGE_H
