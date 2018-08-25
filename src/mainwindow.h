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
    enum class CheckboxState { CHECKED = 2, UNCHECKED = 0 };

    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

   void fillChatMessageListView( const QVector<ChatMessage> *newChatMessages );
   void addChatMessageToListView( const ChatMessage &chatMessage );

private: 
    static QString archivFilename;

    MainWindow( const MainWindow &obj )= delete;
    MainWindow& operator=( const MainWindow &obj ) = delete;

    static bool parseLock;

    Ui::MainWindow *ui;
    qint64  crawlEveryMsec;
    ChatManager *chatManager;

    bool chatAutoScroll;
    int currentSliderEndPos;
    bool loadedArchive;

    void addToListWidgetChat( const QVector<ChatMessage> *newChatMessages );
    void add( QListWidget *list, const ChatMessage chatMessage );
    void remove( QListWidget *list, qint64  messageId );

    void save();
    void load();

signals:

public slots:
    void onMessageChecked( qint64  messageId, int state );
    void onConnectClicked();
    void onDisconnectClicked();
    void onChatChanged();

    void onStopButtonClicked();        // Deprecated
    void onContinueButtonClicked();    // Deprecated

    void onScroll();

    void onDeleteClicked( qint64  messageId, bool isArchivedMessage );
    void onReSendQuestionClicked( const QString &user, const QString &message );
};

#endif // MAINWINDOW_H
