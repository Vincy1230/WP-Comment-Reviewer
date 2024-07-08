#ifndef REPLIER_H
#define REPLIER_H

#include <QDebug>
#include <QDialog>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

namespace Ui {
class Replier;
}

class Replier : public QDialog {
    Q_OBJECT

public:
    explicit Replier(QWidget* parent = nullptr);
    ~Replier();

private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_plainTextEdit_textChanged();

private:
    Ui::Replier* ui;
    QJsonObject* _comment;
    bool approve();
    QJsonObject getMe();
    bool sendreply(QJsonObject me);
};

#endif // REPLIER_H
