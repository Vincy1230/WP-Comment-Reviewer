#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    QJsonObject now_comment;
    int index_1 = -1;
    int index_2 = -1;
    void set_comboBox_1(QMap<QString, QMap<QString, QJsonObject>> commentsByPost);
    void set_comboBox_2(QMap<QString, QJsonObject> comments);
    void set_comment(QJsonObject comment);
    void set_placeholder(QString placeholder = QString());

private slots:
    void on_comboBox_1_textActivated(const QString& arg1);
    void on_comboBox_2_textActivated(const QString& arg1);
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();

private:
    Ui::MainWindow* ui;
    void reset();
    QJsonArray getComments(QString domain, QString user, QString password);
    QMap<QString, QMap<QString, QJsonObject>> getCommentsByPost(QJsonArray comments);
};

#endif // MAINWINDOW_H
