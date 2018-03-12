#include "bamviewer.h"

BamViewer::BamViewer(QWidget *parent)
    :QAbstractScrollArea(parent)
{

    viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
    QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);





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

    setRegion("ecoli", 0, 100);





}

void BamViewer::setAlignementFile(const QString &bamFile)
{

    mAlignementFile = bamFile;


}

void BamViewer::setRegion(const QString &chr, int start, int end)
{
    mRegion.seqName  = chr.toStdString();
    mRegion.beginPos = start;
    mRegion.endPos   = end;

    emit regionChanged();
    updateScrollBar();
    viewport()->update();
}

const seqan::GenomicRegion &BamViewer::region() const
{
    return mRegion;
}

QString BamViewer::regionName() const
{
    QString ret ="test";
    return ret;
}


void BamViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(viewport());

    // test

    QString info = QString("%1:%2-%3").arg(regionName())
                                      .arg(mRegion.beginPos)
                                      .arg(mRegion.endPos);

    painter.setPen(QPen(Qt::lightGray));
    painter.drawText(viewport()->rect(), Qt::AlignCenter|Qt::AlignHCenter, info);



    paintReference(painter);
    paintAlignement(painter);



}

void BamViewer::paintReference(QPainter &painter)
{
//    seqan::Dna5String seq = regionSequence();
//    quint64 length = seqan::length(seq);
//    float step = viewport()->width() / float(length);

//    QPen pen;
//    pen.setWidth(3);
//    painter.setPen(pen);

//    float x = 0;
//     for (seqan::Dna5& nuc : seq)
//     {

//        // painter.drawPoint(x,10);

//         painter.drawText(x,10, QString(char(nuc)));

//         x+=step;
//     }

//    for (int i=0; i< viewport()->width(); i+= step)
//    {

//        painter.drawPoint(i, 10);
//        painter.drawStaticText(i,10, seq[i]);

//    }


}

void BamViewer::paintAlignement(QPainter &painter)
{

//   seqan::BamFileIn bamFileIn;
//   quint64 length = seqan::length(currentReferenceSequence());
//   float step     = viewport()->width() / float(length);


//   if(!open(bamFileIn, alignementFile().toStdString().data()))
//   {
//       qWarning()<<Q_FUNC_INFO<<"cannot open bam file";
//       return;
//   }


//   seqan::BamHeader header;
//   seqan::readHeader(header, bamFileIn);


//   seqan::BamAlignmentRecord record;

//   int row = 2;
//   QFontMetrics metrics(painter.font());

//   while(!seqan::atEnd(bamFileIn))
//   {
//       seqan::readRecord(record, bamFileIn);
//        //TODO :  use jump region .. .
//       // this is just for testing


//       int32_t pos = record.beginPos;
//       int32_t end = pos + seqan::length(record.seq);



//       if ( pos >= mRegion.beginPos && end <= mRegion.endPos)
//       {

//           int x = (pos - mRegion.beginPos) * step;

//           for (const seqan::CharString& nuc : record.seq)
//           {

//              // painter.drawPoint(x,10);

//               QString base = seqan::toCString(nuc);

//               painter.drawText(x, row * metrics.height(), base);

//               x+=step;
//           }

//            row++;
//       }

//   }





}


QString BamViewer::referenceIndexFile() const
{
    return mReferenceFile+".fai";
}

quint64 BamViewer::referenceLength(const QString &referenceName) const
{
    quint64 idx;

    if (referenceName.isEmpty())
        idx = idFromChromosom(mRegion.seqName);
    else
        idx = idFromChromosom(referenceName.toStdString().data());

   return seqan::sequenceLength(mFaiIndex,idx);
}

seqan::Dna5String BamViewer::regionSequence() const
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

    int scrollMin = 0;
    int scrollMax = int(referenceLength() - seqan::length(mRegion));
    horizontalScrollBar()->setRange(scrollMin, scrollMax);


   // horizontalScrollBar()->setPageStep(scrollMax/seqan::length(mRegion));

    horizontalScrollBar()->setValue(mRegion.beginPos);

}

void BamViewer::scrollContentsBy(int dx, int dy)
{
    // dx and dy are negatif ..
    mRegion.beginPos += -dx ;
    mRegion.endPos += -dx;
    emit regionChanged();

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
