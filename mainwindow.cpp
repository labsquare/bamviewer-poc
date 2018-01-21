#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new BamViewer;

    mViewer->setFilename("/home/sacha/exemple.bam");


    setCentralWidget(mViewer);
}

MainWindow::~MainWindow()
{
}
