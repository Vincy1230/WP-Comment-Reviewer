#include "login.h"
#include "about.h"
#include "ui_login.h"

Login::Login(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Login)
    , _settings("./user.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    _settings.beginGroup("User");
    if (_settings.contains("domain")) {
        ui->lineEdit_1->setText(_settings.value("domain").toString());
    }
    if (_settings.contains("user")) {
        ui->lineEdit_2->setText(_settings.value("user").toString());
    }
    if (_settings.contains("password")) {
        ui->lineEdit_3->setText(QByteArray::fromBase64(_settings.value("password").toByteArray()));
    }
    _domain = ui->lineEdit_1->text();
    _user = ui->lineEdit_2->text();
    _password = ui->lineEdit_3->text();
    connect(ui->lineEdit_1, &QLineEdit::textChanged, this, &Login::on_lineEdit_1_textChanged);
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &Login::on_lineEdit_2_textChanged);
    connect(ui->lineEdit_3, &QLineEdit::textChanged, this, &Login::on_lineEdit_3_textChanged);
    refreshApply();
}

Login::~Login()
{
    delete ui;
}

void Login::refreshApply()
{
    ui->pushButton_1->setEnabled(!_domain.isEmpty() && !_user.isEmpty() && !_password.isEmpty());
}

void Login::on_lineEdit_1_textChanged(const QString& domain)
{
    _domain = domain;
    refreshApply();
}

void Login::on_lineEdit_2_textChanged(const QString& user)
{
    _user = user;
    refreshApply();
}

void Login::on_lineEdit_3_textChanged(const QString& password)
{
    _password = password;
    refreshApply();
}

void Login::on_pushButton_1_clicked()
{
    if (ui->checkBox->isChecked()) {
        _settings.setValue("domain", _domain);
        _settings.setValue("user", _user);
        _settings.setValue("password", _password.toUtf8().toBase64().toStdString().c_str());
    } else {
        _settings.remove("domain");
        _settings.remove("user");
        _settings.remove("password");
    }
    extern QString gDomain, gUser, gPassword;
    gDomain = _domain;
    gUser = _user;
    gPassword = _password;
    accept();
}

void Login::on_pushButton_2_clicked()
{
    About about(this);
    about.exec();
}
