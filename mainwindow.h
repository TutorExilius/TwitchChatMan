#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    Ui::MainWindow *ui;

public slots:
    void onMessageChecked( size_t messageId );
};

#endif // MAINWINDOW_H
