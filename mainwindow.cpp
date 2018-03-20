#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bamtrack.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new TrackViewer;
    setCentralWidget(mViewer);

    mViewer->setReference("/tmp/data/ecoli.fa");

    mViewer->addTrack(new BamTrack("/tmp/data/sample1.bam"));



    mViewer->setRegion("ecoli", 1, 100);


    mEdit   = new QLineEdit;
    mSlider = new QSlider(Qt::Horizontal);

    QToolBar * tool = new QToolBar;
    tool->addWidget(mEdit);
    tool->addWidget(mSlider);

    addToolBar(Qt::BottomToolBarArea, tool);


    connect(mViewer, SIGNAL(regionChanged(QString)), mEdit, SLOT(setText(QString)));

    resize(900, 400);

}

MainWindow::~MainWindow()
{
}
