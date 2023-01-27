#include <phonenumber.h>

#include "clientcontroller.h"
#include "accountwidget.h"
#include "ui_accountwidget.h"

AccountWidget::AccountWidget(QWidget *parent, quint64 userId)
    : QWidget(parent)
    , m_ui(new Ui::AccountWidget)
    , m_userId(userId)
{
    m_ui->setupUi(this);

    ClientController::instance().getUserInfoRequest(m_userId, [this](const AbstractMessage& message) {
        auto& getUserInfoAnswerMessage = dynamic_cast<const GetUserInfoAnswerMessage&>(message);
        if (!getUserInfoAnswerMessage.picture.isEmpty())
        {
            QPixmap pixmap;
            pixmap.loadFromData(getUserInfoAnswerMessage.picture);
            m_ui->picturelabel->setPixmap(pixmap);
            m_ui->picturelabel->setMinimumSize(pixmap.size());
        }
        m_ui->firstNameLabel->setText(getUserInfoAnswerMessage.firstname);
        m_ui->lastNameLabel->setText(getUserInfoAnswerMessage.lastname);
        m_ui->phonenumberLabel->setText("+"+phonenumber2String(getUserInfoAnswerMessage.phonenumber));

        adjustSize();
    });
}

AccountWidget::~AccountWidget()
{
    delete m_ui;
}
