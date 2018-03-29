#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bamtrack.h"
#include "oliviertrack.h"

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

    connect(mEdit, &QLineEdit::returnPressed,[this](){

        QRegularExpression exp("(\\w+):(\\d+)-(\\d+)");
        auto match = exp.match(mEdit->text());

        if (match.hasMatch())
        {
            QString name = match.captured(1);
            int start    = match.captured(2).toInt();
            int end      = match.captured(3).toInt();

            mViewer->setRegion(name, start, end);
        }

    });

    resize(900, 400);

}

MainWindow::~MainWindow()
{
}
