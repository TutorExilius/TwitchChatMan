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

    this->ui->checkBox->setVisible( false );
    this->ui->pushButton_reSendQuestion->setVisible( false );

    if( this->chatMessage.getId() == 0 )
    {
        this->ui->pushButton->setVisible( false );
    }

    QObject::connect( this, &CheckableChatMessage::emitChecked,
                      this->mainWindow, &MainWindow::onMessageChecked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->pushButton, &QPushButton::clicked,
                      this, &CheckableChatMessage::onMarkClicked );

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

        this->ui->pushButton->setEnabled( true );
    }
    else
    {
        this->setStyleSheet( "CheckableChatMessage{"
            "border: 1px solid black;"
            "background-color: #aad;"
        "}");

        this->ui->pushButton->setEnabled( false );
    }

    emit emitChecked( this->chatMessage.getId(), state );
}

void CheckableChatMessage::uncheck()
{
    this->ui->checkBox->setCheckState( Qt::CheckState::Unchecked );
    this->onStateChanged( Qt::CheckState::Unchecked );
}

void CheckableChatMessage::hideCheckBox()
{
    this->ui->checkBox->setVisible( false );
}

void CheckableChatMessage::showDeleteButton()
{
    this->ui->pushButton->setText( "&Löschen" );
    //this->ui->pushButton_delete->setVisible( true );
}

void CheckableChatMessage::showReSendQuestionButton()
{
    this->ui->pushButton_reSendQuestion->setVisible( true );
    //this->ui->pushButton_delete->setVisible( true );
}

void CheckableChatMessage::onDeleteClicked()
{
    emit emitDeleteMessage( this->chatMessage.getId(), this->chatMessage.isFromArchive() );
    this->onStateChanged( Qt::CheckState::Unchecked );
}

void CheckableChatMessage::onMarkClicked()
{
 //   emit emitChecked( this->chatMessage.getId(), Qt::CheckState::Checked  );
    this->onStateChanged( Qt::CheckState::Checked );
}

void CheckableChatMessage::registerDeletButton()
{
    QObject::connect( this->ui->pushButton, &QPushButton::clicked,
                      this, &CheckableChatMessage::onDeleteClicked,
                      Qt::UniqueConnection );

    QObject::disconnect( this->ui->pushButton, &QPushButton::clicked,
                      this, &CheckableChatMessage::onMarkClicked );
}

void CheckableChatMessage::registerMarkButton()
{
    QObject::disconnect( this->ui->pushButton, &QPushButton::clicked,
                      this, &CheckableChatMessage::onDeleteClicked);

    QObject::connect( this->ui->pushButton, &QPushButton::clicked,
                      this, &CheckableChatMessage::onMarkClicked,
                      Qt::UniqueConnection );
}

void CheckableChatMessage::registerReSendButton()
{
    QObject::connect( this->ui->pushButton_reSendQuestion, &QPushButton::clicked,
                      this, &CheckableChatMessage::onReSendClicked,
                      Qt::UniqueConnection );
}

void CheckableChatMessage::onReSendClicked()
{
    emitReSendQuestion( this->chatMessage.getUser(),
                        this->chatMessage.getMessage() );
}
