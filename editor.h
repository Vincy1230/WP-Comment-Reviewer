#ifndef EDITER_H
#define EDITER_H

#include <QDebug>
#include <QDialog>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class Editor;
}

class Editor : public QDialog {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);
    ~Editor();

private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Editor* ui;
    QJsonObject* _comment;
};

#endif // EDITER_H
