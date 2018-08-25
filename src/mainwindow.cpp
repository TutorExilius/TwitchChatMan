#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "checkablechatmessage.h"
#include "ircchat.h"

#include <QDateTime>
#include <QDebug>
#include <QException>
#include <QMap>
#include <QListWidgetItem>
#include <QString>
#include <QWidgetItem>
#include <QPushButton>
#include <QScrollBar>
#include <QSlider>

QString MainWindow::archivFilename = "archiv.txt";

MainWindow::MainWindow( QWidget *parent )
: QMainWindow{ parent }
, ui{ new Ui::MainWindow }
//, ircChat{ nullptr }
//, defaultUrl{ R"(https://www.twitch.tv/[PLACEHOLDER]/chat)" }
, chatManager{ nullptr }
, chatAutoScroll{ true }
, currentSliderEndPos{ 0 }
, loadedArchive{ false }
{
    this->chatManager = new ChatManager{ this };

    this->ui->setupUi(this);

    this->load();

    this->ui->horizontalWidget_chatFields->setVisible( false );
    this->resize( this->sizeHint() );

    QObject::connect( this->ui->button_connection, &QPushButton::clicked,
                      this, &MainWindow::onConnectClicked,
                      Qt::UniqueConnection );

  //  QObject::connect( this->ui->listWidget_chat->verticalScrollBar(), &QScrollBar::sliderMoved,
   //                   this, &MainWindow::onScroll );

    QObject::connect( this->ui->listWidget_chat->verticalScrollBar(), &QScrollBar::valueChanged,
                      this, &MainWindow::onScroll );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add( QListWidget *list, const ChatMessage chatMessage )
{
    try
    {
        QListWidgetItem *listWidgetItem = new QListWidgetItem;

        CheckableChatMessage *checkableChatMessage =
                new CheckableChatMessage{ this, chatMessage };
       list->addItem( listWidgetItem );

       if( list->objectName() == "listWidget_checkedMessages" )
       {

            checkableChatMessage->hideCheckBox();
            checkableChatMessage->showDeleteButton();
            checkableChatMessage->showReSendQuestionButton();

            checkableChatMessage->registerDeletButton();
            checkableChatMessage->registerReSendButton();

            QObject::connect( checkableChatMessage, &CheckableChatMessage::emitReSendQuestion,
                              this, &MainWindow::onReSendQuestionClicked,
                              Qt::UniqueConnection );

            QObject::connect( checkableChatMessage, &CheckableChatMessage::emitDeleteMessage,
                              this, &MainWindow::onDeleteClicked,
                              Qt::UniqueConnection );

//checkableChatMessage->update();
//checkableChatMessage->updateGeometry();
//checkableChatMessage->resize( checkableChatMessage->sizeHint() );
       }
       else
       {


           checkableChatMessage->registerMarkButton();
       }

       list->setItemWidget( listWidgetItem, checkableChatMessage );

  /*      checkableChatMessage->setSizePolicy(
                    QSizePolicy( QSizePolicy::Minimum,
                                 QSizePolicy::Minimum ));
*/
        listWidgetItem->setSizeHint( checkableChatMessage->size() );
    }
    catch( QException ex )
    {
        qDebug() << "In 'MainWindow::add( const ChatMessage chatMessage )'\n\t" << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << "In 'MainWindow::add( const ChatMessage chatMessage )'\n\t" << ex;
    }
    catch( ... )
    {
        qDebug() << "In 'MainWindow::add( const ChatMessage chatMessage )'\n\t" << "CRASHED";
    }

    if( list->objectName() == "listWidget_checkedMessages" )
    {
        this->save();
    }
}

void MainWindow::remove( QListWidget *list, qint64 messageId )
{
    try
    {
        this->blockSignals( true );

        bool found = false;
        int itemIndex = 0;

        for (int i = 0; i < list->count(); ++i)
        {
            CheckableChatMessage* message = dynamic_cast<CheckableChatMessage*>(list->itemWidget(list->item(i)));

            if( message != nullptr )
            {
                if( message->getMessageId() == messageId )
                {
                    found = true;
                    itemIndex = i;
                    break;
                }
            }
        }

        if( found )
        {
 //           list->removeItemWidget( deleteItem );
            list->takeItem( itemIndex );
        }

        this->blockSignals( false );
    }
    catch( QException ex )
    {
        qDebug() << "In 'MainWindow::remove( QListWidget *list, qint64 messageId )'\n\t" << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << "In 'MainWindow::remove( QListWidget *list, qint64 messageId )'\n\t" << ex;
    }
    catch( ... )
    {
        qDebug() << "In 'MainWindow::remove( QListWidget *list, qint64 messageId )'\n\t" << "CRASHED";
    }

    this->save();
}


void MainWindow::addToListWidgetChat( const QVector<ChatMessage> *newChatMessages )
{
    try
    {
        for( auto it = newChatMessages->begin(); it != newChatMessages->end(); it++ )
        {
            this->add( this->ui->listWidget_chat, *it );
        }

        const int minCountMessages = 200;
        const int maxCountMessages = 400;
        if( minCountMessages < maxCountMessages )
        {
            QMap<qint64 ,ChatMessage> *chatMessages = this->chatManager->getChatMessages();

            // just keep between minCountMessages and maxCountMessages Messages into List
            if( this->ui->listWidget_chat->count() > maxCountMessages )
            {
                 qDebug() << "should enter";
                this->ui->listWidget_chat->clear();

                // start from minCountMessages'th ChatMessage!
                for( auto it = chatMessages->begin() + ((chatMessages->size()-1) - minCountMessages);
                     it != chatMessages->end();
                     it++ )
                {
                    this->add( this->ui->listWidget_chat, *it );
                }

                qDebug() << "INFO: ChatListView resized to " << this->ui->listWidget_chat->count();
            }
        }

        if( this->chatAutoScroll )
        {
            this->ui->listWidget_chat->scrollToBottom();
            this->currentSliderEndPos = this->ui->listWidget_chat->verticalScrollBar()->sliderPosition();
        }

        this->ui->label_countMessages->setText( "Messages in View: " + QString::number(this->ui->listWidget_chat->count())
                                                + " of " + QString::number( this->chatManager->chatMessagesCount() ) );
    }
    catch( QException ex )
    {
        qDebug() << "In 'MainWindow::addToListWidgetChat( const QVector<ChatMessage> *newChatMessages )'\n\t" << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << "In 'MainWindow::addToListWidgetChat( const QVector<ChatMessage> *newChatMessages )'\n\t" << ex;
    }
    catch( ... )
    {
        qDebug() << "In 'MainWindow::addToListWidgetChat( const QVector<ChatMessage> *newChatMessages )'\n\t" << "CRASHED";
    }
}

// TODO: copy an instance of ChatMessage with that ID in the right listView
void MainWindow::onMessageChecked( qint64  messageId, int state )
{
    this->blockSignals( true );

    qDebug() << "Message-ID checked: " << messageId;

    ChatMessage chatMessage = this->chatManager->getChatMessage( messageId );

    if( static_cast<CheckboxState>(state) == CheckboxState::CHECKED )
    {
        this->add( this->ui->listWidget_checkedMessages, chatMessage );
    }
    else if( static_cast<CheckboxState>(state) == CheckboxState::UNCHECKED )
    {
        this->remove( this->ui->listWidget_checkedMessages, messageId );
    }

    this->blockSignals( false );
}

void MainWindow::onConnectClicked()
{
    this->blockSignals( true );

    QObject::disconnect( this->ui->button_connection, &QPushButton::clicked,
                      this, &MainWindow::onConnectClicked );


    QObject::connect( this->ui->button_connection, &QPushButton::clicked,
                      this, &MainWindow::onDisconnectClicked );

    this->ui->button_connection->setText( "&Disonnect" );
    this->ui->horizontalWidget_chatFields->setHidden( false );
    this->resize( 850, 600 );

    QString currentChannel = "TutorExilius";

    if( this->ui->textEdit_urlToChat->toPlainText().size() > 0 )
    {
        currentChannel = this->ui->textEdit_urlToChat->toPlainText();
    }

    QByteArray channelByteArray{currentChannel.toUtf8()};
    this->chatManager->start( channelByteArray );

    /*
    if( this->chat != nullptr )
    {
        this->chat = new IrcChat{ this };
    }

    this->chat->connectToServer( IrcChat::getConnectionData(), "tutorexilius" );
*/
/*
    if( this->chatView != nullptr )
    {
        const QString chatViewUrl = this->chatView->url().toString();

        if( chatViewUrl == urlToChat )
        {
            currentUrlAlreadyOpened = true;
        }
        else
        {
            this->chatView->close();
            this->chatManager->reset();

            this->ui->label_countMessages->setText( "Messages: 0" );
            this->ui->listWidget_chat->clear();
        }
    }

    if( !currentUrlAlreadyOpened )
    {
        this->chatView = new QWebView;
        this->myPage = new MyWebPage{this};
        this->chatView->setPage( this->myPage );

        QWebSettings *settings =  this->chatView->settings();
        settings->setAttribute(QWebSettings::JavascriptEnabled, true );
        settings->setAttribute(QWebSettings::LocalStorageEnabled, true );

        settings->setAttribute(QWebSettings::PluginsEnabled, false );
        settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, false );
        settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, false );
        settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false );
        settings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, false );
        settings->setAttribute(QWebSettings::Accelerated2dCanvasEnabled, false );
        settings->setAttribute(QWebSettings::TiledBackingStoreEnabled, false );
        settings->setAttribute(QWebSettings::SiteSpecificQuirksEnabled, false );
        settings->setAttribute(QWebSettings::FrameFlatteningEnabled, false );
        settings->setAttribute(QWebSettings::CSSGridLayoutEnabled, false );
        settings->setAttribute(QWebSettings::CSSRegionsEnabled, false );
        settings->setAttribute(QWebSettings::WebGLEnabled, false );
        settings->setAttribute(QWebSettings::HyperlinkAuditingEnabled, false );

        this->chatView->load( QUrl(urlToChat) );

        QObject::connect( this->myPage, &QWebPage::repaintRequested,
                          this->myPage, &MyWebPage::onRepaintRequested,
                          Qt::UniqueConnection );

        QObject::connect( this->myPage, &MyWebPage::myPageRepaintRequested,
                          this, &MainWindow::onChatChanged,
                          Qt::UniqueConnection );
    }

    this->chatView->show();
*/

    this->blockSignals( false );
}

void MainWindow::onDisconnectClicked()
{
    this->blockSignals( true );

    QObject::disconnect( this->ui->button_connection, &QPushButton::clicked,
                          this, &MainWindow::onDisconnectClicked );

    QObject::connect( this->ui->button_connection, &QPushButton::clicked,
                          this, &MainWindow::onConnectClicked );

    this->ui->button_connection->setText( "&Connect" );
    this->ui->horizontalWidget_chatFields->setHidden( true );

    this->resize( 20,20 );

    this->blockSignals( false );
}

void MainWindow::onChatChanged()
{
    try
    {
        static unsigned long long parsedCount = 0;
/*
        if( this->myPage != nullptr )
        {
            const QString html = this->myPage->getLastHtml();

            if( html.size() > 0 )
            {
                if( this->chatManager->parse( html ) )
                {
                    ++parsedCount;

                    qDebug() << "Chat-Crawl No: "
                         << parsedCount << "\tHTML-Page Length: "
                         << html.size();
                }
            }
        }
       */
    }
    catch( QException ex )
    {
        qDebug() << "In 'MainWindow::onChatChanged()'\n\t" << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << "In 'MainWindow::onChatChanged()'\n\t" << ex;
    }
    catch( ... )
    {
        qDebug() << "In 'MainWindow::onChatChanged()'\n\t" << "CRASHED";
    }
}

void MainWindow::fillChatMessageListView( const QVector<ChatMessage> *newChatMessages )
{
    this->addToListWidgetChat( newChatMessages );
    qDebug() << "Messages currently in View:\t" << this->ui->listWidget_chat->count();
}

void MainWindow::addChatMessageToListView( const ChatMessage &chatMessage )
{
    this->add( this->ui->listWidget_chat, chatMessage );
}

void MainWindow::onStopButtonClicked()
{
    qDebug() << "onStopButtonClicked";

   // this->chatView->stop();
}

void MainWindow::onContinueButtonClicked()
{
    qDebug() << "onContinueButtonClicked";
}

void MainWindow::onScroll()
{
    if( this->currentSliderEndPos > this->ui->listWidget_chat->verticalScrollBar()->sliderPosition() )
    {
        this->chatAutoScroll = false;
    }
    else
    {
        this->chatAutoScroll = true;
    }
}


void MainWindow::onDeleteClicked( qint64 messageId, bool isArchivedMessage )
{
    bool found = false;
    int itemIndex = 0;

    auto list = this->ui->listWidget_chat;

    for (int i = 0; i < list->count(); ++i)
    {
        CheckableChatMessage* message = dynamic_cast<CheckableChatMessage*>(list->itemWidget(list->item(i)));

        if( message != nullptr )
        {
            qDebug() << message->getMessageId();
            qDebug() << messageId;

            if( message->getMessageId() == messageId )
            {
                found = true;
                itemIndex = i;

                message->uncheck();

                break;
            }
        }
    }
}

void MainWindow::onReSendQuestionClicked( const QString &user, const QString &message )
{
    QString fullMessage = "[Alte Frage von " + user + "] " + message;
    this->chatManager->write( fullMessage.toUtf8() );
}


void MainWindow::save()
{
    if( loadedArchive )
    {
        QFile file( MainWindow::archivFilename );

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
          QTextStream stream(&file);
          stream.setCodec("UTF-8");
          stream.setGenerateByteOrderMark(false);

          auto list = this->ui->listWidget_checkedMessages;

          for (int i = 0; i < list->count(); ++i)
          {
              CheckableChatMessage* message = dynamic_cast<CheckableChatMessage*>(list->itemWidget(list->item(i)));

              if( message != nullptr )
              {
                  QString out = message->getChatMessage().saveFormat();
                  qDebug() << out ;
                  stream << out;
              }
          }
          stream.flush();

          qDebug() << "saved";
        }
    }
}

void MainWindow::load()
{
    QFile file( MainWindow::archivFilename );

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream.setGenerateByteOrderMark(false);

        QString data = stream.readAll();
        QStringList dataList = data.split( "\n" );


        if( dataList.size() > 4 )
        {
            const int chatMessageLineOffset = 4;

            for( int i = 0; i<dataList.size()-1; i+=chatMessageLineOffset )
            {
                ChatMessage chatMessage{
                    dataList.at(i).toLongLong(),
                    QDateTime::fromString( dataList.at(i+1) ),
                    dataList.at(i+2),
                    dataList.at(i+3),
                    true
                };

                this->add( this->ui->listWidget_checkedMessages, chatMessage );
            }
        }

    }

    this->loadedArchive = true;
}
