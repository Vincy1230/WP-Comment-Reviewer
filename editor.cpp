#include "editor.h"
#include "ui_editor.h"

Editor::Editor(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Editor)
    , _comment(nullptr)
{
    ui->setupUi(this);
    extern QMap<QString, QMap<QString, QJsonObject>> gCommentsByPost;
    extern QString gPostTitle, gCommentTitle;
    _comment = &gCommentsByPost[gPostTitle][gCommentTitle];
    ui->plainTextEdit->setPlainText(_comment->value("content").toObject().value("rendered").toString());
}

Editor::~Editor()
{
    delete ui;
}

void Editor::on_pushButton_1_clicked()
{
    extern QString gDomain, gUser, gPassword;
    extern QString gPostTitle, gCommentTitle;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;

    QString content = ui->plainTextEdit->toPlainText();

    request.setUrl(QUrl("https://" + gDomain + "/wp-json/wp/v2/comments/" + QString::number(_comment->value("id").toInt())));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(gUser).arg(gPassword).toLatin1()).toBase64());

    QJsonObject data;
    data.insert("content", content);
    QJsonDocument doc(data);
    QByteArray body = doc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply = manager.post(request, body);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Edit Failed"));
        qDebug() << reply->errorString();
        return;
    }

    QString newCommentTitle = QString::number(_comment->value("id").toInt()) + " - " + _comment->value("author_name").toString() + " - " + content.simplified();
    gCommentTitle = newCommentTitle.length() > 50 ? newCommentTitle.left(50) : newCommentTitle;

    accept();
}

void Editor::on_pushButton_2_clicked()
{
    reject();
}
