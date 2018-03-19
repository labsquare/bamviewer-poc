#include "bamviewer.h"

BamViewer::BamViewer(QWidget *parent)
    :QAbstractScrollArea(parent)
{

    viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
    QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);

    resize(800,400);


}

void BamViewer::setReferenceFile(const QString &fastaFile)
{
    mReferenceFile    = fastaFile;
    QString indexFile = fastaFile+".bai";

    if (!QFile::exists(mReferenceFile))
        qDebug()<<Q_FUNC_INFO<<mReferenceFile<<" doesn't exists";

    if (!QFile::exists(indexFile))
        qDebug()<<Q_FUNC_INFO<<indexFile<<" doesn't exists";


    if (!open(mFaiIndex, mReferenceFile.toStdString().data()))
        qDebug() << Q_FUNC_INFO<< "ERROR: Could not load FAI index " << indexFile<< "\n";

    updateScrollBar();

}

void BamViewer::setAlignementFile(const QString &bamFile)
{
    mAlignementFile = bamFile;
    QString index   = bamFile+".bai";

    if (!QFile::exists(mAlignementFile))
        qDebug()<<Q_FUNC_INFO<<mAlignementFile<<" doesn't exists";

    if (!QFile::exists(index))
        qDebug()<<Q_FUNC_INFO<<index<<" doesn't exists";

    // Open bam file
    if(!open(mBamFileIn, mAlignementFile.toStdString().data()))
    {
        qWarning()<<Q_FUNC_INFO<<"cannot open bam file";
        return;
    }

    // Open BAI index file
    if (!open(mBaiIndex, QString(mAlignementFile+".bai").toStdString().data()))
    {
        qWarning()<<Q_FUNC_INFO<<"ERROR: Could not read BAI index file ";
        return ;
    }

    // Read header
    seqan::readHeader(mHeader, mBamFileIn);

}
void BamViewer::setRegion(const QString& chr, int start, int end)
{
    mRegion.seqName  = chr.toStdString();
    mRegion.beginPos = start;
    mRegion.endPos   = end;
}

void BamViewer::setRegion(const seqan::GenomicRegion &region)
{
    mRegion = region;
}

int BamViewer::regionLength() const
{
    return mRegion.endPos - mRegion.beginPos;
}

quint64 BamViewer::referenceLength(const QString &chr) const
{
    quint64 idx = idFromChromosom(chr.toStdString().data());
    return seqan::sequenceLength(mFaiIndex,idx);
}

void BamViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(viewport());

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);

    painter.setFont(font);

    paintReference(painter);
    paintAlignement(painter);

}

void BamViewer::paintReference(QPainter &painter)
{
    // paint top reference in viewport.width
    // ACGTATAT........................
    seqan::Dna5String seq = currentReferenceSequence();

    //int step = qFloor(float(viewport()->width()) / float(regionLength()));
    int x    = 0;


    QByteArray ref;

    // seqan to Qt
    std::copy(seqan::begin(seq),
              seqan::end(seq),
              std::back_inserter(ref));


    QFont font = painter.font();
    QFontMetrics metrics(font);

    float step = float( viewport()->width()- metrics.width(ref)) / float(ref.length());

    font.setLetterSpacing(QFont::AbsoluteSpacing, step);
    painter.setFont(font);
    painter.drawText(x,20,ref);


}

void BamViewer::paintAlignement(QPainter &painter)
{
    // jump to region
    bool hasAlignements = false ;
    int rID = idFromChromosom(mRegion.seqName);

    // for testing purpose : get ALL REGION 1-1000
    if (!seqan::jumpToRegion(mBamFileIn, hasAlignements, rID, 1, 1000, mBaiIndex))
    {
        qWarning()<<"could not jump to region";
        return;
    }

    // if no alignement avaible
    if (!hasAlignements)
    {
        qWarning()<<"no alignement here";
        return;
    }


    // Loop over reads and draw it using box packing
    QFontMetrics metrics(painter.font());

    // Clean Pack Reads ..
    mReadPacker.clear();

    // Clean Depth
    mDepths.clear();
    mDepths.fill(0, regionLength());

    // WARNING : => font metrics already set in paintReference
    // float step = float(qAbs(metrics.width(ref) - viewport()->width())) / float(ref.length());


    // We must create 2 loop .
    // One for computation and one for drawing, because we don't want to draw extra reads
    while (!seqan::atEnd(mBamFileIn))
    {
        seqan::BamAlignmentRecord record;
        seqan::readRecord(record, mBamFileIn);

        addRecordToDepth(record);

        int row = mReadPacker.getYRecord(record);

        int delta  = record.beginPos - mRegion.beginPos;
        float x    = delta *  float(viewport()->width()) / float(regionLength());

        // draw reads

        // seqan to Qt

        //QByteArray read;
        QByteArray read(seqan::length(record.seq), 'A');
        std::copy(seqan::begin(record.seq),
                  seqan::end(record.seq),
                  read.begin());

        //        painter.setBrush(QColor("#C9C9FF"));
        //        painter.drawRect(x,(row+2)*  metrics.height(),
        //                         metrics.width(read),
        //                         metrics.height()-4
        //                         );

        if ( (row + 3 ) * metrics.height() < viewport()->height())
            painter.drawText(x,(row+3) *  metrics.height(), read);
    }
}

void BamViewer::addRecordToDepth(const seqan::BamAlignmentRecord& record)
{
    for (int i=record.beginPos; i< seqan::length(record.seq); ++i)
    {
        mDepths[i]++;
        if(mDepths[i] > mMaxDepth)
            mMaxDepth = mDepths[i];
    }
}

quint64 BamViewer::currentReferenceLength() const
{
    quint64 idx = idFromChromosom(mRegion.seqName);
    return seqan::sequenceLength(mFaiIndex,idx);
}


seqan::Dna5String BamViewer::referenceSequence(const seqan::GenomicRegion &region)
{
    seqan::Dna5String sequenceInfix;
    seqan::readRegion(sequenceInfix, mFaiIndex, region);
    return sequenceInfix;
}

seqan::Dna5String BamViewer::currentReferenceSequence()
{
    return referenceSequence(mRegion);
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
    int maxXSize = currentReferenceLength();

    qDebug()<<"max size"<<maxXSize;


    horizontalScrollBar()->setRange(1, maxXSize - viewport()->width());
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



}

const QString &BamViewer::referenceFile() const
{
    return mReferenceFile;
}
