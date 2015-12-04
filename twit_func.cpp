#include "twit_func.h"

twit_func::twit_func(QObject *parent, QCoreApplication *coreApp)
    : QObject(parent)
    ,app(coreApp)
{

}

void twit_func::run(){
    QStringList argv = QCoreApplication::arguments();
    bool input = false;
    if(argv.size() > 1){
        if(argv.at(1) == "input_auth_info"){
            input = true;
        }
    }
    QString consumer_key;
    QString consumer_secret;
    QString oauth_token;
    QString oauth_token_secret;

    QSettings settings("tea_soak_lab", "cli_tweet");
    if(settings.value("error_code").toInt() != 0) input=true;
    if(settings.value("consumer_key").toString() == "") input=true;
    if(input == true){
        if(settings.value("error_code").toInt() != 0){
            std::cout<<"前回投稿に失敗したようです。エラーは以下の通りです。"<<std::endl;
            std::cout<<"code:"<<settings.value("error_code").toInt()<<std::endl;
            std::cout<<"Message:"<<settings.value("error_mess").toString().toStdString()<<std::endl;
        }
        if(settings.value("consumer_key").toString() == ""){
            std::cout<<"設定が存在しないようです。"<<std::endl;
        }
        std::cout<<"認証情報を入力してください。"<<std::endl;
        QTextStream in(stdin);
        std::cout<<"Consumer key: ";
        in>>consumer_key;
        settings.setValue("consumer_key",consumer_key);
        std::cout<<"Consumer secret: ";
        in>>consumer_secret;
        settings.setValue("consumer_secret",consumer_secret);
        std::cout<<"oAuth token: ";
        in>>oauth_token;
        settings.setValue("oauth_token",oauth_token);
        std::cout<<"oAuth token secret: ";
        in>>oauth_token_secret;
        settings.setValue("oauth_token_secret",oauth_token_secret);
    }
    consumer_key = settings.value("consumer_key").toString();
    consumer_secret = settings.value("consumer_secret").toString();
    oauth_token = settings.value("oauth_token").toString();
    oauth_token_secret = settings.value("oauth_token_secret").toString();


    QString update_url = "https://api.twitter.com/1.1/statuses/update.json";

    QString tweet;
    std::cout<<"text:";
    QTextStream in(stdin);
    in>>tweet;
//    std::string tweet_debug = tweet.toUtf8().constData();
//    std::cout<<tweet_debug;

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

    QString reply_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(reply_data.toUtf8());
    if(json.object().value("errors").toArray().at(0).toObject().value("code").toInt() != 0){
        settings.setValue("error_code",json.object().value("errors").toArray().at(0).toObject().value("code").toInt());
        settings.setValue("error_mess",json.object().value("errors").toArray().at(0).toObject().value("message").toString());
    } else {
        settings.setValue("error_code",0);
        settings.setValue("error_mess","none");
    }
    app->quit();
}
