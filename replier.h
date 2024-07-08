#ifndef REPLIER_H
#define REPLIER_H

#include <QDialog>

namespace Ui {
class Replier;
}

class Replier : public QDialog
{
    Q_OBJECT

public:
    explicit Replier(QWidget *parent = nullptr);
    ~Replier();

private:
    Ui::Replier *ui;
};

#endif // REPLIER_H
