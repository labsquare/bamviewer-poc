#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new BamViewer;

    mViewer->setReferenceFile("/tmp/data/ecoli.fa");
    mViewer->setAlignementFile("/tmp/data/sample1.bam");
    mViewer->setRegion("ecoli", 0, 100);



    mEdit   = new QLineEdit;
    mSlider = new QSlider(Qt::Horizontal);

    QToolBar * tool = new QToolBar;
    tool->addWidget(mEdit);
    tool->addWidget(mSlider);

    addToolBar(Qt::BottomToolBarArea, tool);



    setCentralWidget(mViewer);

    resize(900, 400);

}

MainWindow::~MainWindow()
{
}
