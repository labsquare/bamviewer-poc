#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new BamViewer;

    mViewer->setReferenceFile("/home/sacha/Bioinfo/data/hg19.fa");
    mViewer->setRegion("chr3", 100000, 100050);



    setCentralWidget(mViewer);
}

MainWindow::~MainWindow()
{
}
