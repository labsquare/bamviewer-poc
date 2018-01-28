#include <iostream>
#include <algorithm>
#include <seqan/align.h>
#include <seqan/store.h>
#include <QtCore>
#include <QApplication>
#include "mainwindow.h"

using namespace seqan;
using namespace std;

int main(int argc, char *argv[])
{


    QApplication app(argc, argv);


    MainWindow win;
    win.show();


    return app.exec();

//    QDir root = QDir("/home/sacha/Dev/seqan/demos/tutorial/fragment_store");

//    FragmentStore<> store;

//    std::string samFile = root.absoluteFilePath("example.sam").toStdString();
//    std::string fasFile = root.absoluteFilePath("example.fa").toStdString();


//    loadContigs(store, fasFile.c_str());
//    BamFileIn bamFile(samFile.c_str());
//    readRecords(store, bamFile);

//    AlignedReadLayout layout;
//    layoutAlignment(layout, store);

//    printAlignment(std::cout, layout, store, 1, 30, 130, 0, 36);



    return 0;
}
