#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QTableView>
#include <QSqlTableModel>
#include <QHBoxLayout>

#include "database.h"
#include "servercontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTableView *tableview = new QTableView;

    auto model = new QSqlTableModel(nullptr, Database::instance().sqlDatabase());
    model->setTable("users");
    model->select();

    tableview->setModel(model);

    QLabel *lbl = new QLabel;

    QObject::connect(tableview, &QAbstractItemView::clicked, [lbl](const QModelIndex& idx) {
        QByteArray pic;
        Database::instance().userPictureByUserId(idx.row()+1, pic);

        QPixmap pixmap;
        pixmap.loadFromData(pic);
        lbl->setPixmap(pixmap);
    });

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tableview);
    layout->addWidget(lbl);

    QWidget widget;
    widget.setLayout(layout);
    widget.show();

    ServerController::instance();

    return a.exec();
}
