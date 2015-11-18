#ifndef TWIT_FUNC
#define TWIT_FUNC

#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QObject>
#include <QThread>

#include <openssl/sha.h>
#include <openssl/hmac.h>

class twit_func : public QObject
{
    Q_OBJECT

public:
    twit_func(QObject *parent, QCoreApplication* coreApp);

public slots:
    void run();

private:
    QCoreApplication* app;
};

#endif // TWIT_FUNC

