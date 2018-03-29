#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bamtrack.h"
#include "oliviertrack.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new TrackViewer;
    setCentralWidget(mViewer);

    mViewer->setReference("/home/sacha/Linux_data/bioinfo/data/hg19.fa");

    mViewer->addTrack(new BamTrack("/home/sacha/Linux_data/bioinfo/data/bam/IonXpress_001_R_2018_02_06_11_44_40_user_run18C28_Auto_user_run18C28_349.bam"));

    mViewer->setRegion("chr5",112111255,112111467);


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
