#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDateTime>
#include <QDebug>
#include <QWidgetItem>
#include <QSizePolicy>

#include "checkablechatmessage.h"
#include "chatmessage.h"
#include "chatparser.h"

MainWindow::MainWindow( QWidget *parent, const qint64 crawlEveryMsec )
: QMainWindow{ parent }
, ui{ new Ui::MainWindow }
, chatView{ nullptr }
, defaultUrl{ "https://www.twitch.tv/popout/[PLACEHOLDER]/chat" }
, crawlEveryMsec{ crawlEveryMsec }
{
    this->ui->setupUi(this);

    this->ui->horizontalWidget_chatFields->setVisible( false );
    this->resize( this->sizeHint() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Todo: Move-Konstruktor for ChatMessage ?!
void MainWindow::addToListWidgetChat( const ChatMessage *chatMessage )
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem;

    CheckableChatMessage *checkableChatMessage =
            new CheckableChatMessage{ this, chatMessage, this };

    qDebug() << chatMessage->getId() << "\theight:   "
               << checkableChatMessage->size().height();

    this->ui->listWidget_chat->addItem( listWidgetItem );
    this->ui->listWidget_chat->setItemWidget( listWidgetItem, checkableChatMessage );

    qDebug() << chatMessage->getId() << "\theight:   "
               << checkableChatMessage->size().height();

    checkableChatMessage->setSizePolicy(
                QSizePolicy( QSizePolicy::Minimum,
                           QSizePolicy::Minimum ));

    qDebug() << chatMessage->getId() << "\theight:   "
               << checkableChatMessage->size().height();


    listWidgetItem->setSizeHint( checkableChatMessage->size() );

    listWidgetItem->setBackgroundColor( QColor( 222, 242, 255, 150 ) );
}

void MainWindow::onMessageChecked( size_t messageId )
{
    qDebug() << "Message-ID: " << messageId;
}

void MainWindow::onStartChatClicked()
{
    this->ui->horizontalWidget_chatFields->setVisible( true );
    this->resize( 850, 600 );

    QString currentChannel = "TutorExilius";

    if( this->ui->textEdit_urlToChat->toPlainText().size() > 0 )
    {
        currentChannel = this->ui->textEdit_urlToChat->toPlainText();
    }

    QString urlToChat = this->defaultUrl;
    urlToChat.replace( "[PLACEHOLDER]", currentChannel );

    bool currentUrlAlreadyOpened = false;

    if( this->chatView != nullptr )
    {
         if( this->chatView->url().toString() == urlToChat )
         {
             currentUrlAlreadyOpened = true;
         }
         else
         {
             this->chatView->close();
         }
    }

    if( !currentUrlAlreadyOpened )
    {
        this->chatView = new QWebView;
        this->chatView->load(QUrl(urlToChat));
        this->chatView->show();
    }

    connect( this->chatView->page(), &QWebPage::repaintRequested,
             this, &MainWindow::handleChat, Qt::UniqueConnection );
}

void MainWindow::handleChat()
{
    static unsigned long long parsedCount = 0;

    static QDateTime lastDateTime = QDateTime::currentDateTime();
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 milliSecDiff = lastDateTime.msecsTo( currentDateTime );

    static size_t lastHtmlSize = 0;

    if( milliSecDiff > this->crawlEveryMsec )
    {
        const size_t size = this->chatView->page()->currentFrame()->toHtml().size();

        if( lastHtmlSize != size )
        {
            lastDateTime = currentDateTime;

            //delay = 0;

            lastHtmlSize = size;
            ++parsedCount;

            qDebug() << "Chat-Crawl No: "
                 << parsedCount << "\tHTML-Page Length: "
                 << size;

            const QString html = this->chatView->page()->currentFrame()->toHtml();

            const QVector<ChatMessage*> *newChatMessages =
                    this->collectNewChatMessages( html );

            if( newChatMessages != nullptr )
            {
                this->currentChatMessages.append( *newChatMessages );
                this->updateChatMessageListView( newChatMessages );
            }
        }
    }
}

const QVector<ChatMessage*>* MainWindow::collectNewChatMessages( const QString &html )
{
    QVector<ChatMessage*> *newChatMessages = nullptr;
    // TODO: parse html and fill vector with new messages

    ChatParser *parser = new ChatParser;
    parser->parse( html );

    if( parser->isOk() )
    {
        newChatMessages = parser->popParsedChatMessages();
    }

    return const_cast<const QVector<ChatMessage*>*>( newChatMessages );
}

void MainWindow::updateChatMessageListView( const QVector<ChatMessage*> *newChatMessages )
{
    for( unsigned int i=0; i<newChatMessages->size(); i++ )
    {
        this->addToListWidgetChat( newChatMessages->at(i) );
    }
}
