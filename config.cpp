#include "config.h"

#include <QDebug>
#include <QFile>

Config::Config( const QString &configFilePath, QObject *parent )
: QObject{ parent }
, configFile{ nullptr }
{
    this->configFile = new QFile{ configFilePath };

    if( !(this->configFile->exists()) )
    {
        qDebug() << "config-file: '" << configFilePath << "' is missing";
        throw "config-file: '" + configFilePath + "' is missing";
    }
}

const ConnectionData* Config::getConnectionData() const
{
    if( this->configFile->open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        int lineCount = 0;

        QByteArray nick;
        QByteArray pass;

        while( !(this->configFile->atEnd()) )
        {
            ++lineCount;

            QByteArray line = this->configFile->readLine();

            if( line.size() == 0 )
            {
                return nullptr;
            }

            switch( lineCount )
            {
            case 1:
                nick = line;
                break;

            case 2:
                pass = line;
                break;
            }
        }

        return new ConnectionData{ nick, pass };
    }

    return nullptr;
}

