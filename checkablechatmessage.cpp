#include "checkablechatmessage.h"
#include "ui_checkablechatmessage.h"

#include <QDebug>
#include <QTextEdit>
#include <QFontMetrics>

CheckableChatMessage::CheckableChatMessage(
        const ChatMessage &chatMessage,
        const MainWindow *mainWindow,
        QWidget *parent )
: QFrame{ parent }
, ui{ new Ui::CheckableChatMessage }
, chatMessage{ chatMessage }
, mainWindow{ mainWindow }
{
    this->ui->setupUi(this);

    QObject::connect( this, &CheckableChatMessage::emitChecked,
              this->mainWindow, &MainWindow::onMessageChecked );

    QObject::connect( this->ui->textEdit, &QTextEdit::textChanged,
              this, &CheckableChatMessage::onTextChanged );

    this->ui->label_user->setText( chatMessage.getUser() );
    this->ui->label_time->setText(
                chatMessage.getDateTime().time().toString() );
    this->ui->textEdit->setText( chatMessage.getMessage() );
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

    emit emitChecked( this->chatMessage.getId() );
}


void CheckableChatMessage::onTextChanged()
{
    qDebug() << "onTextChanged() called";

}
