#include "checkablechatmessage.h"

#include "ui_checkablechatmessage.h"
#include "chatmessage.h"

#include <QDebug>

CheckableChatMessage::CheckableChatMessage(
        QWidget *parent,
        const ChatMessage chatMessage )
: QFrame{ parent }
, ui{ new Ui::CheckableChatMessage }
, mainWindow{ dynamic_cast<MainWindow*>( parent ) }
, chatMessage{ chatMessage }
{
    if( this->mainWindow == nullptr )
    {
        qDebug() << "Parent of CheckableChatMessage must be a pointer of MainWindow!";
        throw "Parent of CheckableChatMessage must be a pointer of MainWindow!";
    }

    this->ui->setupUi(this);

    QObject::connect( this, &CheckableChatMessage::emitChecked,
                      this->mainWindow, &MainWindow::onMessageChecked,
                      Qt::UniqueConnection );

    const QString dateTime_username = chatMessage.getDateTime().time().toString() + "   " +chatMessage.getUser();

    this->ui->label->setText(dateTime_username
                             + "\n\n"
                             + chatMessage.getMessage() );

    this->resize( this->sizeHint() );
}

CheckableChatMessage::~CheckableChatMessage()
{
    delete ui;
}

void CheckableChatMessage::onStateChanged( int state )
{
    qDebug() << state ;

    if( state == 0 )
    {
        this->setStyleSheet( "CheckableChatMessage{"
            "border: 1px solid black;"
            "background-color: white;"
         "}");
    }
    else
    {
        this->setStyleSheet( "CheckableChatMessage{"
            "border: 1px solid black;"
            "background-color: #aad;"
        "}");
    }

    emit emitChecked( this->chatMessage.getId() );
}
