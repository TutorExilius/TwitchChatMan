#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDateTime>
#include <QDebug>
#include <QWidgetItem>

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
                "User3", "Test3" };

    QListWidgetItem *item = new QListWidgetItem;
    CheckableChatMessage *chatMessage =
            new CheckableChatMessage{ message1, this };

    this->ui->listWidget->addItem( item );
    this->ui->listWidget->setItemWidget( item, chatMessage );
    item->setSizeHint( chatMessage->size() );

    QListWidgetItem *item2 = new QListWidgetItem;
    CheckableChatMessage *chatMessage2 =
            new CheckableChatMessage{ message2, this };

    this->ui->listWidget->addItem( item2 );
    this->ui->listWidget->setItemWidget( item2, chatMessage2 );
    item2->setSizeHint( chatMessage2->size() );

    QListWidgetItem *item3 = new QListWidgetItem;
    CheckableChatMessage *chatMessage3 =
            new CheckableChatMessage{ message3, this };

    this->ui->listWidget->addItem( item3 );
    this->ui->listWidget->setItemWidget( item3, chatMessage3 );
    item3->setSizeHint( chatMessage3->size() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMessageChecked( size_t messageId )
{
    qDebug() << "Message-ID: " << messageId;
}

