#include <QApplication>
#include <widget.h>

#include "fen.h"
#include <iostream>


using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
