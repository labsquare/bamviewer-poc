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


    std::stringstream stream;
    stream << seq;
    QString ref = QString::fromStdString(stream.str());


    QFont font = painter.font();
    QFontMetrics metrics(font);

    float step = float( viewport()->width()- metrics.width(ref)) / float(ref.length());

    font.setLetterSpacing(QFont::AbsoluteSpacing, step);
    painter.setFont(font);
    painter.drawText(x,20,ref);


}

void BamViewer::paintAlignement(QPainter &painter)
{
    // paint top reference in viewport.width
    // ACGTATAT........................

    seqan::BamFileIn bamFileIn;

    // Open bam file
    if(!open(bamFileIn, alignementFile().toStdString().data()))
    {
        qWarning()<<Q_FUNC_INFO<<"cannot open bam file";
        return;
    }

    // Open BAI index file
    seqan::BamIndex<seqan::Bai> baiIndex;
    if (!open(baiIndex, QString(alignementFile()+".bai").toStdString().data()))
    {
        qWarning()<<Q_FUNC_INFO<<"ERROR: Could not read BAI index file ";
        return ;
    }


    // Read header
    seqan::BamHeader header;
    seqan::readHeader(header, bamFileIn);



    // jump to region
    bool hasAlignements = false ;
    int rID = idFromChromosom(mRegion.seqName);


    // for testing purpose : get ALL REGION 1-1000
    if (!seqan::jumpToRegion(bamFileIn, hasAlignements, rID, 1, 1000, baiIndex))
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


    // Pack Reads ..
    mReadPacker.clear();

    // WARNING : => font metrics already set in paintReference
    // float step = float(qAbs(metrics.width(ref) - viewport()->width())) / float(ref.length());

    while (!seqan::atEnd(bamFileIn))
    {
        seqan::BamAlignmentRecord record;
        seqan::readRecord(record, bamFileIn);

        int row = mReadPacker.getYRecord(record);

        int delta  = record.beginPos - mRegion.beginPos;
        float x    = delta *  float(viewport()->width()) / float(regionLength());

        std::stringstream stream;
        stream << record.seq;

        // draw reads

        QString read = QString::fromStdString(stream.str());

//        painter.setBrush(QColor("#C9C9FF"));
//        painter.drawRect(x,(row+2)*  metrics.height(),
//                         metrics.width(read),
//                         metrics.height()-4
//                         );

        painter.drawText(x,(row+3) *  metrics.height(), QString::fromStdString(stream.str()));


//        for (const seqan::CharString& nuc : record.seq)
//        {
//            painter.drawText(x,(row+3) *  metrics.height(), seqan::toCString(nuc));
//            x+= step;
//        }

        //        seqan::Dna5String ref = currentReferenceSequence();
        //        seqan::Align<seqan::Dna5String> align;
        //        seqan::bamRecordToAlignment(align, ref, record );

    }

    // Draw reads ..


    //        float step = float(viewport()->width()) / float(regionLength());

    //        int delta =  record.beginPos- mRegion.beginPos;

    //        float x    = delta *  float(viewport()->width()) / float(regionLength());




    //        for (const seqan::CharString& nuc : record.seq)
    //        {

    //                painter.drawText(x,row *  metrics.height(), seqan::toCString(nuc));
    //                x+= step;
    //        }

    //        row++;

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
