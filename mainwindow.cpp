#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new BamViewer;

    mViewer->setReferenceFile("/home/sacha/Bioinfo/data/hg19.fa");



    setCentralWidget(mViewer);

    resize(900, 400);

}

MainWindow::~MainWindow()
{
}
