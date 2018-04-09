#include "referencetrack.h"

ReferenceTrack::ReferenceTrack(const QString &filename)
    :AbstractTrack()
{

    setFilename(filename);


}

void ReferenceTrack::setFilename(const QString &filename)
{
    if (filename.isEmpty())
        return;

    qDebug()<<"load reference : "<<filename;

    mFastaFile        = filename;
    QString indexFile = filename+".fai";

    if (!QFile::exists(mFastaFile))
        qDebug()<<Q_FUNC_INFO<<mFastaFile<<" doesn't exists";

    if (!QFile::exists(indexFile))
        qDebug()<<Q_FUNC_INFO<<indexFile<<" doesn't exists";


    if (!open(mFaiIndex, mFastaFile.toStdString().data()))
        qDebug() << Q_FUNC_INFO<< "ERROR: Could not load FAI index " << indexFile<< "\n";

    qDebug()<<chromosoms();

}

quint64 ReferenceTrack::baseCount(const seqan::CharString &chromosom) const
{
    quint64 idx = chromosomToId(chromosom);
    return seqan::sequenceLength(mFaiIndex,idx);
}

quint64 ReferenceTrack::baseCount() const
{
    return baseCount(viewer()->region().seqName);
}


quint64 ReferenceTrack::chromosomToId(const seqan::CharString &chromosom) const
{
    quint64 idx = 0;
    if (!seqan::getIdByName(idx, mFaiIndex, chromosom)){

        std::cout<<Q_FUNC_INFO<<"error fai index has no entry for "<<chromosom<<std::endl;
    }
    return idx;
}

QStringList ReferenceTrack::chromosoms() const
{
    QStringList list;
    for (int i = 0 ; i< seqan::numSeqs(mFaiIndex); ++i)
    {
        qDebug()<<i;
        std::stringstream stream;
        seqan::CharString sname = seqan::sequenceName(mFaiIndex, i);
        stream << sname;
        list.append(QString::fromStdString(stream.str()));
    }
    return list;
}

seqan::Dna5String ReferenceTrack::sequence(const seqan::GenomicRegion &region)
{
    seqan::Dna5String sequenceInfix;
    seqan::readRegion(sequenceInfix, mFaiIndex, region);
    return sequenceInfix;
}

seqan::Dna5String ReferenceTrack::sequence()
{
    return sequence(viewer()->region());
}

void ReferenceTrack::paint(QPainter *painter, seqan::GenomicRegion &region, int width)
{
    // paint top reference in viewport.width
    // ACGTATAT........................

    seqan::Dna5String seq = sequence();
    int x    = 0;
    QByteArray ref(seqan::length(seq), 'A');
    // seqan to Qt
    std::copy(seqan::begin(seq),
              seqan::end(seq),
              ref.begin());

    QFont font = painter->font();
    QFontMetrics metrics(font);

    float letterSpacing = float( width - metrics.width(ref)) / float(ref.length());

    //painter->setBrush(Qt::red);
    //painter->drawRect(0,0, width, 20);
    font.setLetterSpacing(QFont::AbsoluteSpacing, letterSpacing);


    // Draw rect
    int letterWidth = metrics.width('A');
    QPen pen = painter->pen();
    pen.setWidth(1);
    pen.setColor(Qt::transparent);
    painter->setPen(pen);


    float xbox = x - metrics.width('A')/2;
    float ybox = 0;
    float wbox = float(width) / float(ref.length());
    bool alt = 0;

    //    QHash<QChar, QColor> colorsHash = {
    //        {'A',QColor("#009000")},    // green
    //        {'C',QColor("#0000ff")},    // blue
    //        {'G',QColor("#000000")},    // black
    //        {'T',QColor("#ff0000")},    // red
    //        {'W',QColor("#804800")},    // mix of A and T
    //        {'S',QColor("#000080")},    // mix of C and G
    //        {'M',QColor("#004880")},    // mix of A and C
    //        {'K',QColor("#800000")},    // mix of G and T
    //        {'R',QColor("#004800")},    // mix of A and G
    //        {'Y',QColor("#800080")},    // mix of C and T
    //        {'B',QColor("#550055")},    // mix of C, G, and T
    //        {'D',QColor("#553000")},    // mix of A, G, and T
    //        {'H',QColor("#553055")},    // mix of A, C, and T
    //        {'V',QColor("#003055")},    // mix of A, C, and G
    //        {'N',QColor("#999")}        // gray
    //    };


    int index = 0;
    while (xbox < width && index < ref.length())
    {

        //painter->setBrush(colorsHash[ref.at(index)]);
        painter->drawRect(xbox, ybox, wbox, 20);
        xbox += wbox;

        alt = !alt;
        index++;



    }

    pen.setColor(Qt::black);
    painter->setPen(Qt::black);
    painter->setFont(font);
    painter->drawText(x,20,ref);



}

int ReferenceTrack::height()
{
    QFont font;
    QFontMetrics metrics(font);

    return metrics.height();
}


