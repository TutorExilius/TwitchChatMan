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

private slots:
    void onStateChanged( int state );

signals:
    void emitChecked( uint messageId );

private:
    CheckableChatMessage() = delete;
    CheckableChatMessage( const CheckableChatMessage& ) = delete;
    CheckableChatMessage& operator=( const CheckableChatMessage& ) = delete;

    Ui::CheckableChatMessage *ui;
    size_t id;

    const MainWindow *mainWindow;
    const ChatMessage chatMessage;
};

#endif // CHECKABLECHATMESSAGE_H
