#include "checkablechatmessage.h"
#include "ui_checkablechatmessage.h"

#include <QDebug>
#include <QTextEdit>
#include <QFontMetrics>

CheckableChatMessage::CheckableChatMessage(
        QWidget *parent,
        const ChatMessage *chatMessage,
        const MainWindow *mainWindow)
: QFrame{ parent }
, ui{ new Ui::CheckableChatMessage }
, chatMessage{ chatMessage }
, mainWindow{ mainWindow }
{
    this->ui->setupUi(this);

    QObject::connect( this, &CheckableChatMessage::emitChecked,
              this->mainWindow, &MainWindow::onMessageChecked );

    const QString dateTime_username = chatMessage->getDateTime().time().toString() + "   " +chatMessage->getUser();

    this->ui->label->setText(dateTime_username
                                + "\n\n"
                                + chatMessage->getMessage() );
}

CheckableChatMessage::~CheckableChatMessage()
{
    QObject::disconnect( this, &CheckableChatMessage::emitChecked,
              this->mainWindow, &MainWindow::onMessageChecked );

    delete ui;
}

void CheckableChatMessage::onStateChanged( int state )
{
    qDebug() << state ;

    emit emitChecked( this->chatMessage->getId() );
}
