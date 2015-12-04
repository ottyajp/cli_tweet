#ifndef TWIT_FUNC
#define TWIT_FUNC

#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QStringList>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QRegularExpression>
#include <QCommandLineParser>

#include <openssl/sha.h>
#include <openssl/hmac.h>

extern bool input;
extern bool single;

class twit_func : public QObject
{
    Q_OBJECT

public:
    twit_func(QObject *parent, QCoreApplication* coreApp);
    void post(QString);

public slots:
    void run();

private:
    QCoreApplication* app;
    QString consumer_key;
    QString consumer_secret;
    QString oauth_token;
    QString oauth_token_secret;
};


#endif // TWIT_FUNC

