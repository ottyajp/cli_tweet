#include "twit_func.h"

twit_func::twit_func(QObject *parent, QCoreApplication *coreApp)
    : QObject(parent)
    ,app(coreApp)
{

}

void twit_func::run(){
    QString consumer_key = "2pbR3eRsNfyjpsJw7Xs7JQ";
    QString consumer_secret = "YLVys9QYBnXFV8DE6XeyA2jJNukTaNSoVZm6uMIa0hc";
    QString oauth_token = "94249173-kS9KCoRK3y00zYz4lMb0KpqDcoyy9mWZPlT99Y54c";
    QString oauth_token_secret = "CSO4Khpew2LwuyTFO5yjubQIuZyZcR9YoP37ibV3BA";

    QString update_url = "https://api.twitter.com/1.1/statuses/update.json";

    QTextStream qstdin(stdin);
    QString tweet;
/*    while(!qstdin.atEnd()){
        tweet = qstdin.readLine();
        if(tweet.isEmpty()){
            break;
        } else {
        }
    }*/
    std::cout<<"text:";
    QTextStream in(stdin);
    in>>tweet;
    std::string tweet_debug = tweet.toUtf8().constData();
    std::cout<<tweet_debug;

    qsrand(QDateTime::currentDateTime().toTime_t());
    QString nonce = QString::number(qrand());

    QMap<QString,QString> params;
    params["oauth_consumer_key"] = consumer_key;
    params["oauth_nonce"] = nonce;
    params["oauth_signature_method"] = "HMAC-SHA1";
    params["oauth_timestamp"] = QString::number(time(NULL));
    params["oauth_token"] = oauth_token;
    params["oauth_version"] = "1.0";
    params["status"] = QString::fromUtf8(tweet.toUtf8().toPercentEncoding());

    QString params_str;

    QMapIterator<QString,QString> i(params);
    while(i.hasNext()){
        i.next();
        params_str += QString("%1=%2&").arg(i.key()).arg(i.value());
    }
    params_str.resize(params_str.size() -1);

    QByteArray message = "POST&" + update_url.toUtf8().toPercentEncoding() + "&" + params_str.toUtf8().toPercentEncoding();

    QByteArray key = consumer_secret.toUtf8() + "&" + oauth_token_secret.toUtf8();

    unsigned char res[SHA_DIGEST_LENGTH + 1];
    unsigned int reslen;
    HMAC(EVP_sha1(), (unsigned char*)key.data(), key.length(), (unsigned char*)message.data(), message.length(), res, &reslen);
    std::string result;
    result = (char*)res;
    result.erase(reslen);

    QByteArray signature = QByteArray::fromRawData(result.c_str(), result.length()).toBase64();

    params.remove("status");
    params["oauth_signature"] = QString::fromUtf8(signature.toPercentEncoding());

    QString header_params_str = "";
    QMapIterator<QString,QString> i2(params);
    while(i2.hasNext()){
        i2.next();
        header_params_str += i2.key() + "=" + i2.value() + ",";
    }
    header_params_str.resize(header_params_str.size() -1);

QEventLoop eventLoop;

    QNetworkAccessManager *manager = new QNetworkAccessManager();

QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    //QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QUrl url(update_url);
    QNetworkRequest r(url);

    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    r.setRawHeader(QObject::tr("Authorization").toUtf8(), (QObject::tr("OAuth ") + header_params_str).toUtf8());

    QNetworkReply *reply = manager->post(r, QObject::tr("status=").toUtf8() + tweet.toUtf8().toPercentEncoding());

    eventLoop.exec();

    //qDebug()<<reply->readAll();
    app->quit();
}
