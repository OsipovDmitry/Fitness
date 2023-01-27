#ifndef ACCOUNTWIDGET_H
#define ACCOUNTWIDGET_H

#include <QWidget>

namespace Ui {
class AccountWidget;
}

class AccountWidget : public QWidget
{
    Q_OBJECT

public:
    AccountWidget(QWidget *parent = 0, quint64 = 0);
    ~AccountWidget();

private:
    Ui::AccountWidget *m_ui;
    quint64 m_userId;
};

#endif // ACCOUNTWIDGET_H
