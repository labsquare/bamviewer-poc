#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mViewer = new BamViewer;

    //mViewer->setReferenceFile("/tmp/data/ecoli.fa");
    mViewer->setReferenceFile("/home/sacha/ecoli.fa");
    mViewer->setAlignementFile("/tmp/data/sample1.bam");

    mSlider = new QSlider(Qt::Horizontal);
    mEdit   = new QLineEdit();

    QToolBar * bar = new QToolBar;
    bar->addWidget(mSlider);
    bar->addWidget(mEdit);

    addToolBar(Qt::BottomToolBarArea,bar);

    setCentralWidget(mViewer);

    resize(900, 400);


    connect(mViewer, &BamViewer::regionChanged, this, &MainWindow::updateRegion);

    connect(mEdit, &QLineEdit::returnPressed, [this](){

       QRegularExpression exp("(?<chr>\\w+):(?<start>\\d+)-(?<end>\\d+)");

       QRegularExpressionMatch match = exp.match(mEdit->text());

       if (match.hasMatch())
       {
            QString name = match.captured("chr");
            int start    = match.captured("start").toInt();
            int end      = match.captured("end").toInt();

            mViewer->setRegion(name, start,end);
       }

    });



    updateRegion();
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateRegion()
{

        mEdit->setText(QString("%1:%2-%3").arg(seqan::toCString(mViewer->region().seqName)).
                                           arg(mViewer->region().beginPos).
                                           arg(mViewer->region().endPos));



}
