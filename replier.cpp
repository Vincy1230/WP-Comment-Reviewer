#include "replier.h"
#include "ui_replier.h"

Replier::Replier(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Replier)
{
    ui->setupUi(this);
}

Replier::~Replier()
{
    delete ui;
}
