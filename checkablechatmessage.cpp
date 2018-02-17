#include "checkablechatmessage.h"
#include "ui_checkablechatmessage.h"

#include "chatmessage.h"

#include <QDebug>
#include <QTextEdit>
#include <QFontMetrics>

CheckableChatMessage::CheckableChatMessage(
        QWidget *parent,
        const ChatMessage *chatMessage )
: QFrame{ parent }
, ui{ new Ui::CheckableChatMessage }
, mainWindow{ dynamic_cast<MainWindow*>( parent ) }
, chatMessage{ chatMessage }
{
    if( this->mainWindow == nullptr )
    {
        throw "Parent of CheckableChatMessage must be a pointer of MainWindow!";
    }

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
