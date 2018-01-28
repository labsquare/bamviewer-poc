#include "bamviewer.h"

BamViewer::BamViewer(QObject *parent)
{




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





    //    CharString pathToFile = "/media/sacha/Linux_data/bioinfo/data/hg19.fa";
    //    CharString pathToFaiFile = "/media/sacha/Linux_data/bioinfo/data/hg19.fa.fai";
    //    FaiIndex faiIndex;

    //    if (!build(faiIndex, toCString(pathToFile)))
    //        std::cout << "ERROR: Could not build the index!\n";

    //    if (!save(faiIndex, toCString(pathToFaiFile)))
    //        std::cout << "ERROR: Could not save the index to file!\n";


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


    std::cout<<reference()<<std::flush;


}

QString BamViewer::referenceIndexFile() const
{
    return mReferenceFile+".fai";
}

Dna5String BamViewer::reference()
{

    unsigned idx = idFromChromosom(mRegion.seqName);

    Dna5String sequenceInfix;

    readRegion(sequenceInfix, mFaiIndex, mRegion);


    return sequenceInfix;

}

const QString &BamViewer::alignementFile() const
{
    return mAlignementFile;
}

quint64 BamViewer::idFromChromosom(const CharString &chromosom)
{
    quint64 idx = 0;
    if (!seqan::getIdByName(idx, mFaiIndex, chromosom))
        qDebug()<<Q_FUNC_INFO<<"error fai index has no entry for ";

    return idx;
}

const QString &BamViewer::referenceFile() const
{
    return mReferenceFile;
}
