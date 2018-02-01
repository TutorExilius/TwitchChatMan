#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDateTime>
#include <QDebug>
#include <QWidgetItem>
#include <QSizePolicy>

#include "checkablechatmessage.h"
#include "chatmessage.h"

MainWindow::MainWindow( QWidget *parent )
: QMainWindow{ parent }
, ui{ new Ui::MainWindow }
{
    ui->setupUi(this);

    ChatMessage message1{ 1, QDateTime::currentDateTime(),
                "User1", "Test1" };

    ChatMessage message2{ 2, QDateTime::currentDateTime(),
                "User2", "Test2" };

    ChatMessage message3{ 3, QDateTime::currentDateTime(),
                "User3", "Test3\nfdsgsdfgdsfgsdfgdfgfd\n"
"sdkjffdgsdfgdfsgsdfgdfsgdsfgdsfhajkfg"};

   this->addToListWidgetChat( message1 );
   this->addToListWidgetChat( message2 );
   this->addToListWidgetChat( message3 );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMessageChecked( size_t messageId )
{
    qDebug() << "Message-ID: " << messageId;
}

// Todo: Move-Konstruktor for ChatMessage ?!
void MainWindow::addToListWidgetChat( const ChatMessage &chatMessage )
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem;

    CheckableChatMessage *checkableChatMessage =
            new CheckableChatMessage{ chatMessage, this };



    this->ui->listWidget_chat->addItem( listWidgetItem );
    this->ui->listWidget_chat->setItemWidget( listWidgetItem, checkableChatMessage );

    qDebug() << chatMessage.getId() << "\theight:   "
               << checkableChatMessage->size().height();

    checkableChatMessage->setSizePolicy(
                QSizePolicy( QSizePolicy::Minimum,
                           QSizePolicy::Minimum ));

    qDebug() << chatMessage.getId() << "\theight:   "
               << checkableChatMessage->size().height();


    listWidgetItem->setSizeHint( checkableChatMessage->size() );
}

