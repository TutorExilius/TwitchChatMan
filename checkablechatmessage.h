#ifndef CHECKABLECHATMESSAGE_H
#define CHECKABLECHATMESSAGE_H

#include <QFrame>
#include "mainwindow.h"
#include "chatmessage.h"

namespace Ui {
class CheckableChatMessage;
}

class CheckableChatMessage : public QFrame
{
    Q_OBJECT

public:
    explicit CheckableChatMessage(
                   const ChatMessage &chatMessage,
                   const MainWindow *mainWindow,
                   QWidget *parent = nullptr );
    ~CheckableChatMessage();

private slots:
    void onStateChanged( int state );

signals:
    void emitChecked( size_t messageId );

private:
    Ui::CheckableChatMessage *ui;
    size_t id;
    const MainWindow *mainWindow;

    ChatMessage chatMessage;
};

#endif // CHECKABLECHATMESSAGE_H
