#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>

#include "ircchat.h" // struct ConnectionData

// Forward-Declarations
class QFile;

class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config( const QString &configFilePath,
                     QObject *parent = nullptr );

    const ConnectionData *getConnectionData() const;

signals:

public slots:

private:
    QFile *configFile;
};

#endif // CONFIG_H
