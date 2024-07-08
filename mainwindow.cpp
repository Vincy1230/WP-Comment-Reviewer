#include "mainwindow.h"
#include "about.h"
#include "editor.h"
#include "replier.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":icon/favicon.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_comboBox_1(QMap<QString, QMap<QString, QJsonObject>> commentsByPost)
{
    extern QString gPostTitle;
    ui->comboBox_1->clear();
    for (auto it = commentsByPost.begin(); it != commentsByPost.end(); ++it) {
        ui->comboBox_1->addItem(it.key());
    }
    on_comboBox_1_textActivated(ui->comboBox_1->currentText());
}

void MainWindow::set_comboBox_2(QMap<QString, QJsonObject> comments)
{
    extern QString gCommentTitle;
    ui->comboBox_2->clear();
    if (comments.isEmpty()) {
        ui->comboBox_2->setEnabled(false);
    } else {
        ui->comboBox_2->setEnabled(true);
    }
    for (auto it = comments.begin(); it != comments.end(); ++it) {
        ui->comboBox_2->addItem(it.key());
    }
    on_comboBox_2_textActivated(ui->comboBox_2->currentText());
}

void MainWindow::set_comment(QJsonObject comment)
{
    ui->plainTextEdit->clear();
    if (comment.isEmpty()) {
        ui->pushButton_1->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
    } else {
        set_placeholder();
        ui->plainTextEdit->setPlainText(comment.value("content").toObject().value("rendered").toString());
        ui->pushButton_1->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
    }
}

void MainWindow::on_comboBox_1_textActivated(const QString& arg1)
{
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle;
    gPostTitle = arg1;
    set_comboBox_2(gCommentsByPost[arg1]);
}

void MainWindow::on_comboBox_2_textActivated(const QString& arg1)
{
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;
    gCommentTitle = arg1;
    set_comment(gCommentsByPost[gPostTitle][arg1]);
}

void MainWindow::on_pushButton_1_clicked()
{
    // POST /wp/v2/comments/<id> <- status: approved

    extern QString gDomain, gUser, gPassword;
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/comments/" + QString::number(gCommentsByPost[gPostTitle][gCommentTitle].value("id").toInt())));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

    QJsonObject data;
    data.insert("status", "approved");
    QJsonDocument doc(data);
    QByteArray body = doc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    reply = manager.post(request, body);

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Approval Failed"));
        qDebug() << reply->errorString();
        return;
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reset();
}

void MainWindow::on_pushButton_2_clicked()
{
    extern QString gDomain, gUser, gPassword;
    if (gUser.indexOf('@') == -1) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Posting replies requires logging in with email address"));
        return;
    }
    Replier replier(this);
    if (replier.exec() == QDialog::Accepted) {
        reset();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;
    QJsonObject now_comment = gCommentsByPost[gPostTitle][gCommentTitle];
    Editor editor(this);
    if (editor.exec() == QDialog::Accepted) {
        reset();
        QString postTitle = gPostTitle;
        QString commentTitle = gCommentTitle;
        set_comboBox_1(gCommentsByPost);
        ui->comboBox_1->setCurrentText(postTitle);
        on_comboBox_1_textActivated(postTitle);
        ui->comboBox_2->setCurrentText(commentTitle);
        on_comboBox_2_textActivated(commentTitle);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    // POST /wp/v2/comments/<id> <- status: trash

    extern QString gDomain, gUser, gPassword;
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/comments/" + QString::number(gCommentsByPost[gPostTitle][gCommentTitle].value("id").toInt())));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

    QJsonObject data;
    data.insert("status", "trash");
    QJsonDocument doc(data);
    QByteArray body = doc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    reply = manager.post(request, body);

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Deletion Failed"));
        qDebug() << reply->errorString();
        return;
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reset();
}

void MainWindow::on_pushButton_5_clicked()
{
    // POST /wp/v2/comments/<id> <- status: spam

    extern QString gDomain, gUser, gPassword;
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/comments/" + QString::number(gCommentsByPost[gPostTitle][gCommentTitle].value("id").toInt())));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

    QJsonObject data;
    data.insert("status", "spam");
    QJsonDocument doc(data);
    QByteArray body = doc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    reply = manager.post(request, body);

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Deletion Failed"));
        qDebug() << reply->errorString();
        return;
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reset();
}

void MainWindow::on_pushButton_6_clicked()
{
    reset();
}

void MainWindow::on_pushButton_7_clicked()
{
    About about(this);
    about.exec();
}

void MainWindow::reset()
{
    extern QString gDomain, gUser, gPassword;
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;

    set_placeholder(QString(QObject::tr("[ Refreshing... ]")));
    set_comment(QJsonObject());

    QJsonArray comments = getComments(gDomain, gUser, gPassword);
    if (comments.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("All comments have been approved"));
        // return;
    }

    gCommentsByPost = getCommentsByPost(comments);

    if (gCommentsByPost.contains(gPostTitle) && gCommentsByPost[gPostTitle].contains(gCommentTitle)) {
        set_comment(gCommentsByPost[gPostTitle][gCommentTitle]);
    } else if (gCommentsByPost.contains(gPostTitle)) {
        set_comboBox_2(gCommentsByPost[gPostTitle]);
    } else {
        set_comboBox_1(gCommentsByPost);
    }
}

void MainWindow::set_placeholder(QString placeholder)
{

    ui->plainTextEdit->setPlaceholderText(placeholder);
}

QJsonArray MainWindow::getComments(QString domain, QString user, QString password)
{
    set_placeholder(QString(QObject::tr("[ Refreshing... ]")));
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    request.setUrl(QUrl("https://" + domain + "/wp-json/wp/v2/comments?status=hold&per_page=100"));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(user).arg(password).toLatin1()).toBase64());

    reply = manager.get(request);

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed to read comments"));
        qDebug() << reply->errorString();
        return QJsonArray();
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.array();
}

QMap<QString, QMap<QString, QJsonObject>> MainWindow::getCommentsByPost(QJsonArray comments)
{
    set_placeholder(QString("[ Reading post title... ]"));
    // 遍历评论，列举出现的所有文章ID并去重
    QSet<int> postIds;
    for (int i = 0; i < comments.size(); i++) {
        postIds.insert(comments.at(i).toObject().value("post").toInt());
    }

    // 建立文章ID和到文章标题的映射，查 /wp-json/wp/v2/posts/{id} 获取文章，标题在json中的title字段中的rendered字段中
    QMap<int, QString> postName;
    for (int postId : postIds) {
        QNetworkAccessManager manager;
        QNetworkRequest request;
        QNetworkReply* reply;

        extern QString gDomain, gUser, gPassword;
        request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/posts/" + QString::number(postId)));
        request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

        reply = manager.get(request);

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed to read post title"));
            qDebug() << reply->errorString();
            return QMap<QString, QMap<QString, QJsonObject>>();
        }

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QString title = doc.object().value("title").toObject().value("rendered").toString();
        postName.insert(postId, QString::number(postId) + " - " + title);
        set_placeholder(QString("[ Identified post ") + title + QString(" ]"));
    }

    // 评论按文章标题分类
    QMap<QString, QMap<QString, QJsonObject>> commentsByPost;
    for (int i = 0; i < comments.size(); i++) {
        QJsonObject comment = comments.at(i).toObject();
        QString postTitle = postName.value(comment.value("post").toInt());
        QString commentTitle = QString::number(comment.value("id").toInt()) + " - " + comment.value("author_name").toString() + " - " + comment.value("content").toObject().value("rendered").toString().simplified();
        commentsByPost[postTitle][commentTitle.length() > 50 ? commentTitle.left(50) : commentTitle] = comment;
    }

    set_placeholder(QString(QObject::tr("[ Loading complete ]")));
    return commentsByPost;
}
