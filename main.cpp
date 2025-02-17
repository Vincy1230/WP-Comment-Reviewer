#include "login.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QTranslator>

QString gDomain, gUser, gPassword;
QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
QString gPostTitle, gCommentTitle;

QJsonArray getComments(QString domain, QString user, QString password, MainWindow* w);
QMap<QString, QMap<QString, QJsonObject>> getCommentsByPost(QJsonArray comments, MainWindow* w);

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QLocale locale;
    qApp->installTranslator(&translator);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (locale.language() == QLocale::Chinese && (locale.territory() == QLocale::China || locale.territory() == QLocale::Singapore)) {
#else
    if (locale.language() == QLocale::Chinese && (locale.country() == QLocale::China || locale.country() == QLocale::Singapore)) {
#endif
        if (!translator.load(":/i18n/zh_CN.qm")) {
            qApp->removeTranslator(&translator);
        }
    } else if (locale.language() == QLocale::Chinese) {
        if (!translator.load(":/i18n/zh_TW.qm")) {
            qApp->removeTranslator(&translator);
        }
    } else if (locale.language() == QLocale::Thai) {
        if (!translator.load(":/i18n/th_TH.qm")) {
            qApp->removeTranslator(&translator);
        }
    } else {
        qApp->removeTranslator(&translator);
    }

    MainWindow w;

    w.show();

    Login login(&w);
    if (login.exec() == QDialog::Rejected) {
        return 0;
    }

    extern QString gDomain, gUser, gPassword;

    QJsonArray comments = getComments(gDomain, gUser, gPassword, &w);
    if (comments.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("No unapproved comments"));
        return 0;
    }

    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    gCommentsByPost = getCommentsByPost(comments, &w);

    w.set_comboBox_1(gCommentsByPost);

    return a.exec();
}

QJsonArray getComments(QString domain, QString user, QString password, MainWindow* w)
{
    w->set_placeholder(QString(QObject::tr("[ Reading comments... ]")));

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + domain + "/wp-json/wp/v2/comments?status=hold&per_page=100"));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(user).arg(password).toLatin1()).toBase64());

    reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed to read comments"));
        qDebug() << reply->errorString();
        return QJsonArray();
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.array();
}

QMap<QString, QMap<QString, QJsonObject>> getCommentsByPost(QJsonArray comments, MainWindow* w)
{
    w->set_placeholder(QString(QObject::tr("[ Reading post title... ]")));
    QSet<int> postIds;
    for (int i = 0; i < comments.size(); i++) {
        postIds.insert(comments.at(i).toObject().value("post").toInt());
    }

    QMap<int, QString> postName;
    for (int postId : postIds) {
        QNetworkAccessManager manager;
        QNetworkRequest request;
        QNetworkReply* reply;

        extern QString gDomain, gUser, gPassword;
        request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/posts/" + QString::number(postId)));
        request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

        reply = manager.get(request);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed to read post title"));
            qDebug() << reply->errorString();
            return QMap<QString, QMap<QString, QJsonObject>>();
        }

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QString title = doc.object().value("title").toObject().value("rendered").toString();
        postName.insert(postId, QString::number(postId) + " - " + title);
        w->set_placeholder(QString(QObject::tr("[ Identified post ")) + title + QString(" ]"));
    }

    QMap<QString, QMap<QString, QJsonObject>> commentsByPost;
    for (int i = 0; i < comments.size(); i++) {
        QJsonObject comment = comments.at(i).toObject();
        QString postTitle = postName.value(comment.value("post").toInt());
        QString commentTitle = QString::number(comment.value("id").toInt()) + " - " + comment.value("author_name").toString() + " - " + comment.value("content").toObject().value("rendered").toString().simplified();
        commentsByPost[postTitle][commentTitle.length() > 50 ? commentTitle.left(50) : commentTitle] = comment;
    }

    w->set_placeholder(QString(QObject::tr("[ Loading complete ]")));
    return commentsByPost;
}
