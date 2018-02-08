#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebKitWidgets>

#include "chatmessage.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr, const qint64 crawlEveryMsec = 100 );
    ~MainWindow();

private: 
    Ui::MainWindow *ui;
    QWebView *chatView;
    const QString defaultUrl;
    qint64 crawlEveryMsec;
    QVector<ChatMessage*> currentChatMessages;

    // Todo: Move-Konstruktor for ChatMessage ?!
    void addToListWidgetChat( const ChatMessage *chatMessage );
    const QVector<ChatMessage*>* collectNewChatMessages( const QString &html );
    void updateChatMessageListView( const QVector<ChatMessage*> *newChatMessages );

public slots:
    void onMessageChecked( size_t messageId );
    void onStartChatClicked();
    void handleChat();
};

#endif // MAINWINDOW_H
