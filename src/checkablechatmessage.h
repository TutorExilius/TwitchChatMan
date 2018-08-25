#ifndef CHECKABLECHATMESSAGE_H
#define CHECKABLECHATMESSAGE_H

#include "chatmessage.h"
#include "mainwindow.h"

#include <QFrame>

namespace Ui
{
    class CheckableChatMessage;
}

class CheckableChatMessage : public QFrame
{
    Q_OBJECT

public:
    explicit CheckableChatMessage( QWidget *parent,
                   const ChatMessage chatMessage );
    ~CheckableChatMessage();

    ChatMessage getChatMessage() const
    {
        return this->chatMessage;
    }

    qint64 getMessageId() const
    {
        return this->chatMessage.getId();
    }

    void uncheck();
    void hideCheckBox();
    void showDeleteButton();
    void showReSendQuestionButton();

    void registerDeletButton();
    void registerMarkButton();
    void registerReSendButton();

private slots:
    void onStateChanged( int state );
    void onDeleteClicked();
    void onMarkClicked();
    void onReSendClicked();

signals:
    void emitChecked( qint64  messageId, int state );
    void emitDeleteMessage( qint64  messageId, bool isArchivedMessage );
    void emitReSendQuestion( QString user, QString message );

private:
    CheckableChatMessage() = delete;
    CheckableChatMessage( const CheckableChatMessage& ) = delete;
    CheckableChatMessage& operator=( const CheckableChatMessage& ) = delete;

    Ui::CheckableChatMessage *ui;
   // size_t id;

    const MainWindow *mainWindow;
    const ChatMessage chatMessage;
};

#endif // CHECKABLECHATMESSAGE_H
