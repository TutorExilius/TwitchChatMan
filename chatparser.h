#ifndef CHATPARSER_H
#define CHATPARSER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QString>

// Forward-Declaration
class ChatMessage;

class ChatParser : public QObject
{
    Q_OBJECT

public:
    explicit ChatParser( QObject *parent );

    bool isOk() const;
    void parse( const QString &html );
    void addChatMessage( const QString &chatMessageHtmlPart );
    QVector<ChatMessage*>* popParsedChatMessages();

signals:

public slots:

private:
    ChatParser() = delete;
    ChatParser( const ChatParser &obj ) = delete;
    ChatParser& operator=( const ChatParser &obj ) = delete;

    bool ok;
    qulonglong lastMessagedId;
    QMap<qulonglong, ChatMessage*> *parsedMessages;
    QList<QString*> *messages;
};

#endif // CHATPARSER_H
