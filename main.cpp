#include <iostream>
#include <algorithm>
#include <seqan/align.h>
#include <seqan/store.h>
#include <QtCore>
#include <QDebug>
#include <QApplication>
#include "mainwindow.h"

#include <QContiguousCache>

#include <numeric>

using namespace seqan;
using namespace std;

class MonTest;
class MonTest
{
public:
    MonTest(int i): mI(i) {qDebug()<<"create";}
    ~MonTest() {qDebug()<<"delete";}
    int i() {return mI;}
private:
    int mI = 0;

};

int main(int argc, char *argv[])
{


    QApplication app(argc, argv);

    QContiguousCache<MonTest*> cache;
    cache.setCapacity(5);

    for (int i = 0; i< 100; i++)
    {
           cache.append(new MonTest(i));
    }

    //cache.insert(400, new MonTest(400));


        MainWindow win;
        win.show();


        qDebug()<<std::numeric_limits<quint32>::max();

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


}
