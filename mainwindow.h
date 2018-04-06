#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chatmanager.h"
#include "chatmessage.h"

#include <QMainWindow>

// Forward-Declarations

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

   void updateChatMessageListView( const QVector<ChatMessage> *newChatMessages );

private: 
    MainWindow( const MainWindow &obj )= delete;
    MainWindow& operator=( const MainWindow &obj ) = delete;

    static bool parseLock;

    Ui::MainWindow *ui;
    const QString defaultUrl;
    qint64 crawlEveryMsec;
    ChatManager *chatManager;

    bool chatAutoScroll;

    void addToListWidgetChat( const QVector<ChatMessage> *newChatMessages );
    void add( const ChatMessage chatMessage );

signals:

public slots:
    void onMessageChecked( uint messageId );
    void onConnectClicked();
    void onDisconnectClicked();
    void onChatChanged();

    void onStopButtonClicked();        // Deprecated
    void onContinueButtonClicked();    // Deprecated
};

#endif // MAINWINDOW_H
