#ifndef BAMVIEWER_H
#define BAMVIEWER_H
#include <QAbstractScrollArea>
#include <QPainter>
#include <QtCore>

#include <seqan/seq_io.h>
#include <seqan/bam_io.h>
#include <seqan/sequence.h>


using namespace seqan;



class BamViewer : public QAbstractScrollArea
{
public:
    BamViewer(QObject * parent = nullptr);

    void setReferenceFile(const QString& fastaFile);
    void setAlignementFile(const QString& bamFile);
    void setRegion(const QString& chr, quint64 start, quint64 end);



    const QString &referenceFile() const;
    const QString &alignementFile() const;

    quint64 idFromChromosom(const seqan::CharString& chromosom);

protected:

    void paintEvent(QPaintEvent * event)  override;

    QString referenceIndexFile() const;

    seqan::Dna5String currentSequence();




private:
    QString mReferenceFile;
    QString mAlignementFile;
    seqan::GenomicRegion mRegion;
    seqan::FaiIndex mFaiIndex;


};

#endif // BAMVIEWER_H
