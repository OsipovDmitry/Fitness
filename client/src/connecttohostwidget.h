#ifndef CONNECTTOHOSTWIDGET_H
#define CONNECTTOHOSTWIDGET_H

#include <QWidget>

namespace Ui {
class ConnectToHostWidget;
}

class ConnectToHostWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectToHostWidget(QWidget *parent = 0);
    ~ConnectToHostWidget();

    void tryToConnect();

signals:
    void sgnConnected();

private slots:
    void on_tryAgainButton_clicked();

private:
    void failedConnection();

    Ui::ConnectToHostWidget *m_ui;
};

#endif // CONNECTTOHOSTWIDGET_H
