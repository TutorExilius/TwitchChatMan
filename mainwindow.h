#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chatmanager.h"
#include "chatmessage.h"

#include <QMainWindow>
#include <QListWidget>

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

   void fillChatMessageListView( const QVector<ChatMessage> *newChatMessages );

private: 
    MainWindow( const MainWindow &obj )= delete;
    MainWindow& operator=( const MainWindow &obj ) = delete;

    static bool parseLock;

    Ui::MainWindow *ui;
    const QString defaultUrl;
    qint64 crawlEveryMsec;
    ChatManager *chatManager;

    bool chatAutoScroll;
    int currentSliderEndPos;

    void addToListWidgetChat( const QVector<ChatMessage> *newChatMessages );
    void add( QListWidget *list, const ChatMessage chatMessage );

signals:

public slots:
    void onMessageChecked( uint messageId );
    void onConnectClicked();
    void onDisconnectClicked();
    void onChatChanged();

    void onStopButtonClicked();        // Deprecated
    void onContinueButtonClicked();    // Deprecated

    void onScroll();
};

#endif // MAINWINDOW_H
