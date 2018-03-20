#include "bamtrack.h"
#include "referencetrack.h"

BamTrack::BamTrack(const QString filename)
{
    if (!filename.isEmpty())
        setFilename(filename);
}

void BamTrack::setFilename(const QString &filename)
{
    mFilename = filename;

    if (mFilename.isEmpty())
        return;

    QString index   = mFilename+".bai";

    if (!QFile::exists(mFilename))
        qDebug()<<Q_FUNC_INFO<<mFilename<<" doesn't exists";

    if (!QFile::exists(index))
        qDebug()<<Q_FUNC_INFO<<index<<" doesn't exists";

    // Open bam file
    if(!seqan::open(mBamFileIn, mFilename.toStdString().data()))
    {
        qWarning()<<Q_FUNC_INFO<<"cannot open bam file";
        return;
    }

    // Open BAI index file
    if (!seqan::open(mBaiIndex, index.toStdString().data()))
    {
        qWarning()<<Q_FUNC_INFO<<"ERROR: Could not read BAI index file ";
        return ;
    }

    // Read header
    seqan::readHeader(mHeader, mBamFileIn);

}

void BamTrack::paint(QPainter *painter, seqan::GenomicRegion &region, int width)
{
    // jump to region
    bool hasAlignements = false ;
    int rID = viewer()->referenceTrack()->chromosomToId(region.seqName);

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
    QFontMetrics metrics(painter->font());

    // Clean Pack Reads ..
    mReadPacker.clear();

    // Clean Depth
    mDepths.clear();
    mDepths.resize(viewer()->regionLength());
    mDepths.fill(0);

    qDebug()<<"size" <<mDepths.size();
    // TODO: We must create 2 loop .
    // TODO: One for computation and one for drawing, because we don't want to draw extra reads

    while (!seqan::atEnd(mBamFileIn))
    {
        seqan::BamAlignmentRecord record;
        seqan::readRecord(record, mBamFileIn);

        addRecordToDepth(record);

        int row = mReadPacker.getYRecord(record);

        int delta  = record.beginPos - region.beginPos;
        float x    = delta *  float(width) / float(viewer()->regionLength());

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

        if ((row+5)  * metrics.height() < viewer()->viewport()->height())
            painter->drawText(x, (row+5) *  metrics.height(), read);
    }


    // draw depth testing ....
    float x =  0;
    float r =  float(width) / float(viewer()->regionLength());
    for (int i = 0 ; i < region.endPos - region.beginPos; i++ )
    {

        painter->save();
        QFont font = painter->font();
        font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
        font.setPixelSize(10);
        painter->setFont(font);
        painter->drawText(x,0, QString::number(mDepths[i]));
        painter->restore();
        x += r ;

    }



}

int BamTrack::height()
{
    return   20;
}

void BamTrack::addRecordToDepth(const seqan::BamAlignmentRecord &record)
{



    for (int i=record.beginPos; i<= record.beginPos+seqan::length(record.seq); ++i)
    {
        if ( i >= viewer()->region().beginPos && i <= viewer()->region().endPos)
        {
            int index = i - viewer()->region().beginPos;

                mDepths[index]++;
                if(mDepths[index] > mMaxDepth)
                    mMaxDepth = mDepths[index];


        }


    }
}
