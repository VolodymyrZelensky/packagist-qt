#include "search.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    search w;
    w.show();
    return a.exec();
}
