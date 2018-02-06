#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new BamViewer;

    mViewer->setReferenceFile("/tmp/data/ecoli.fa");



    setCentralWidget(mViewer);

    resize(900, 400);

}

MainWindow::~MainWindow()
{
}
