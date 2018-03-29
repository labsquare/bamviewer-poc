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

    float step = float( width - metrics.width(ref)) / float(ref.length());

    font.setLetterSpacing(QFont::AbsoluteSpacing, step);
    painter->setFont(font);
    painter->drawText(x,20,ref);



}

int ReferenceTrack::height()
{
    QFont font;
    QFontMetrics metrics(font);

    return metrics.height();
}


