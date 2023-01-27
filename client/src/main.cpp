#include <QApplication>
#include <QWidget>

#include "clientmainwindow.h".h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientMainWindow w;
    w.show();

    return a.exec();
}
