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

quint64 BamViewer::regionLength() const
{
    return mRegion.endPos - mRegion.beginPos;
}

void BamViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(viewport());


    paintReference(painter);
    paintAlignement(painter);



}

void BamViewer::paintReference(QPainter &painter)
{

    seqan::Dna5String seq = currentReferenceSequence();

    float step = float(viewport()->width()) / float(regionLength());
    float x    = 0;

    for (auto& c : seq)
    {

        painter.drawText(x,20,QString(char(c)));
        x+=step;

    }
}

void BamViewer::paintAlignement(QPainter &painter)
{

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


    // Loop over reads and draw it
    QFontMetrics metrics(painter.font());


    QList<QList<seqan::BamAlignmentRecord> > rows;


    while (!seqan::atEnd(bamFileIn))
    {

        //        seqan::Dna5String ref = currentReferenceSequence();
        //        seqan::Align<seqan::Dna5String> align;
        //        seqan::bamRecordToAlignment(align, ref, record );

        seqan::BamAlignmentRecord record;
        seqan::readRecord(record, bamFileIn);

        bool newRow = true;

        for (auto& row : rows)
        {
            seqan::BamAlignmentRecord rec = row.last();
            if (rec.beginPos + seqan::length(rec.seq) + 10 > record.beginPos)
            {
                row.append(record);
                newRow = false;
                break;
            }
        }
        if (newRow)
        {
            QList<seqan::BamAlignmentRecord> row;
            row.append(record);
            rows.append(row);
        }


    }



    for (int r = 0; r < rows.size(); ++r)
    {

        for (const auto& rec : rows[r])
        {
            float step = float(viewport()->width()) / float(regionLength());
            int delta  = rec.beginPos- mRegion.beginPos;
            float x    = delta *  float(viewport()->width()) / float(regionLength());

            for (const seqan::CharString& nuc : rec.seq)
            {

                painter.drawText(x,(r+3) *  metrics.height(), seqan::toCString(nuc));
                x+= step;
            }

        }

    }





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
