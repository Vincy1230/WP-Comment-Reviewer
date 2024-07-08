#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class Login;
}

class Login : public QDialog {
    Q_OBJECT

public:
    explicit Login(QWidget* parent = nullptr);
    ~Login();

private slots:
    void on_lineEdit_1_textChanged(const QString& domain);
    void on_lineEdit_2_textChanged(const QString& user);
    void on_lineEdit_3_textChanged(const QString& password);
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Login* ui;
    QString _domain, _user, _password;
    void refreshApply();
    QSettings _settings;
};

#endif // LOGIN_H
