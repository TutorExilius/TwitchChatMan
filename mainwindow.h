#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chatmessage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

private:

    // Todo: Move-Konstruktor for ChatMessage ?!
    void addToListWidgetChat( const ChatMessage &chatMessage );

    Ui::MainWindow *ui;

public slots:
    void onMessageChecked( size_t messageId );
};

#endif // MAINWINDOW_H
