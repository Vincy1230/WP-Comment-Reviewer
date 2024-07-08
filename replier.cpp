#include "replier.h"
#include "ui_replier.h"

Replier::Replier(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Replier)
    , _comment(nullptr)
{
    ui->setupUi(this);
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;
    _comment = &gCommentsByPost[gPostTitle][gCommentTitle];
}

Replier::~Replier()
{
    delete ui;
}

void Replier::on_plainTextEdit_textChanged()
{
    ui->pushButton_1->setEnabled(!ui->plainTextEdit->toPlainText().isEmpty());
}

void Replier::on_pushButton_1_clicked()
{
    if (!approve()) {
        return;
    }
    ui->pushButton_1->setEnabled(false);
    if (!sendreply(getMe())) {
        return;
    }
    accept();
}

void Replier::on_pushButton_2_clicked()
{
    reject();
}

bool Replier::approve()
{
    extern QString gDomain, gUser, gPassword;
    extern QString gPostTitle, gCommentTitle;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/comments/" + QString::number(_comment->value("id").toInt())));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

    QJsonObject data;
    data.insert("status", "approved");
    QJsonDocument doc(data);
    QByteArray body = doc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply = manager.post(request, body);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Approval Failed"));
        qDebug() << reply->errorString();
        return false;
    }

    return true;
}

QJsonObject Replier::getMe()
{
    extern QString gDomain, gUser, gPassword;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/users/me"));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

    reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed to read user information"));
        qDebug() << reply->errorString();
        return QJsonObject();
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}

bool Replier::sendreply(QJsonObject me)
{
    if (me.isEmpty()) {
        return false;
    }

    extern QString gDomain, gUser, gPassword;
    extern QString gPostTitle, gCommentTitle;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/comments"));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

    QJsonObject data;
    data.insert("author", me.value("id").toInt());
    data.insert("author_email", gUser);
    data.insert("author_name", me.value("name").toString());
    data.insert("author_url", gDomain);
    data.insert("content", ui->plainTextEdit->toPlainText());
    data.insert("parent", _comment->value("id").toInt());
    data.insert("post", _comment->value("post").toInt());
    data.insert("status", "approved");
    QJsonDocument doc(data);
    QByteArray body = doc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply = manager.post(request, body);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed to send reply"));
        qDebug() << reply->errorString();
        return false;
    }

    return true;
}
