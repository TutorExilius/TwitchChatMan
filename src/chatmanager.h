#ifndef CHATPARSER_H
#define CHATPARSER_H

#include <QMap>
#include <QObject>
#include <QString>

// Forward-Declaration
class ChatMessage;
class MainWindow;
class IrcChat;
class Parser;

class ChatManager : public QObject
{
    Q_OBJECT

public:
    explicit ChatManager( QObject *parent );
    ~ChatManager();

    //bool parse( const QString &html );
    //bool collectNewMessages( /*Parser *parser*/ );

    void reset();

    QMap<qint64 , ChatMessage> *getChatMessages() const
    {
        return this->chatMessages;
    }

    ChatMessage getChatMessage( qint64  messageId ) const;

    int chatMessagesCount() const
    {
        return this->chatMessages->size();
    }

    void start( const QByteArray &joiningChannel );
    void addChatMessageToList( const QString &message );
    void write( const QString &text );

signals:

public slots:
    void readIrcChatData();

private:
    ChatManager() = delete;
    ChatManager( const ChatManager &obj ) = delete;
    ChatManager& operator=( const ChatManager &obj ) = delete;

    void deleteAllChatMessages();
    void resetParser();

    MainWindow *mainWindow;
    IrcChat *ircChat;
    Parser *parser;
    qint64  lastMessagedId;
    QMap<qint64 , ChatMessage> *chatMessages;
    bool successfullyJoinedChannel;

 //   ChatMessage getChatMessage( const uint &messageId ) const;

};

#endif // CHATPARSER_H
