#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QException>
#include <QDebug>

int main( int argc, char *argv[] )
{
    int ret = 0;

    try
    {
        QApplication a(argc, argv);

// Hat vermutlich Gründe
#ifdef QT_DEBUG
        QDir::setCurrent( QCoreApplication::applicationDirPath() );
#endif

        MainWindow w;
        w.show();
        ret = a.exec();
    }
    catch( QException ex )
    {
        qDebug() << ex.what();
    }
    catch( const char *ex )
    {
        qDebug() << ex;
    }
    catch( ... )
    {
        qDebug() << "CRASHED";
    }

    return 0;
}
