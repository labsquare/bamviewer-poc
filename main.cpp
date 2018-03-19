#include <iostream>
#include <algorithm>
#include <QtCore>
#include <QDebug>
#include <QApplication>
#include "mainwindow.h"

#include <QContiguousCache>
using namespace std;


int main(int argc, char *argv[])
{


    QApplication app(argc, argv);



    MainWindow win;
    win.show();


    return app.exec();



}
