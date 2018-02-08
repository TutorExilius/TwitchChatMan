#ifndef CHATPARSER_H
#define CHATPARSER_H

#include <QObject>

// Forward-Declaration
class ChatMessage;

class ChatParser : public QObject
{
    Q_OBJECT

public:
    explicit ChatParser( QObject *parent = nullptr );

    bool isOk() const;
    void parse( const QString &html );
    QVector<ChatMessage*>* popParsedChatMessages();

signals:

public slots:

private:
    bool ok;
    qulonglong lastMessagedId;
    QVector<ChatMessage*> *parsedMessages;
};

#endif // CHATPARSER_H
