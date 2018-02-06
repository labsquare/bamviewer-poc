#include "bamviewer.h"

BamViewer::BamViewer(QWidget *parent)
    :QAbstractScrollArea(parent)
{

    viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
    QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);



    setRegion("seq", 0, 100);


}

void BamViewer::setReferenceFile(const QString &fastaFile)
{

    mReferenceFile = fastaFile;

    if (!QFile::exists(mReferenceFile))
        qDebug()<<Q_FUNC_INFO<<mReferenceFile<<" doesn't exists";

    if (!QFile::exists(referenceIndexFile()))
        qDebug()<<Q_FUNC_INFO<<referenceIndexFile()<<" doesn't exists";


    if (!open(mFaiIndex, mReferenceFile.toStdString().data()))
        qDebug() << Q_FUNC_INFO<< "ERROR: Could not load FAI index " << referenceIndexFile() << "\n";


    updateScrollBar();




}

void BamViewer::setAlignementFile(const QString &bamFile)
{

    mAlignementFile = bamFile;


}

void BamViewer::setRegion(const QString &chr, quint64 start, quint64 end)
{
    mRegion.seqName  = chr.toStdString();
    mRegion.beginPos = start;
    mRegion.endPos   = end;
}

void BamViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(viewport());


    paintReference(painter);



}

void BamViewer::paintReference(QPainter &painter)
{
    seqan::Dna5String seq = currentReferenceSequence();
    quint64 length = seqan::length(seq);
    float step = viewport()->width() / float(length);

    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);

    float x = 0;
     for (seqan::Dna5& nuc : seq)
     {

        // painter.drawPoint(x,10);

         painter.drawText(x,10, QString(char(nuc)));

         x+=step;
     }

//    for (int i=0; i< viewport()->width(); i+= step)
//    {

//        painter.drawPoint(i, 10);
//        painter.drawStaticText(i,10, seq[i]);

//    }


}


QString BamViewer::referenceIndexFile() const
{
    return mReferenceFile+".fai";
}

quint64 BamViewer::currentReferenceSize() const
{
   unsigned idx = idFromChromosom(mRegion.seqName);
   return seqan::sequenceLength(mFaiIndex,idx);
}

seqan::Dna5String BamViewer::currentReferenceSequence()
{

    seqan::Dna5String sequenceInfix;
    seqan::readRegion(sequenceInfix, mFaiIndex, mRegion);
    return sequenceInfix;

}

const QString &BamViewer::alignementFile() const
{
    return mAlignementFile;
}

quint64 BamViewer::idFromChromosom(const seqan::CharString &chromosom) const
{
    quint64 idx = 0;
    if (!seqan::getIdByName(idx, mFaiIndex, chromosom))
        std::cout<<Q_FUNC_INFO<<"error fai index has no entry for "<<chromosom<<std::endl<<std::flush;

    return idx;
}

void BamViewer::updateScrollBar()
{
    int maxXSize = currentReferenceSize();

    qDebug()<<"max size"<<maxXSize;


    horizontalScrollBar()->setRange(0, maxXSize - viewport()->width());
    horizontalScrollBar()->setPageStep(viewport()->width());
}

void BamViewer::scrollContentsBy(int dx, int dy)
{
    // dx and dy are negatif ..
    mRegion.beginPos += -dx ;
    mRegion.endPos += -dx;

    viewport()->update();

}

void BamViewer::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    updateScrollBar();
}

const QString &BamViewer::referenceFile() const
{
    return mReferenceFile;
}
