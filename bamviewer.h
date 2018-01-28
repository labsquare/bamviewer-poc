#ifndef BAMVIEWER_H
#define BAMVIEWER_H
#include <QAbstractScrollArea>
#include <QtWidgets>
#include <QPainter>
#include <QtCore>

#include <seqan/seq_io.h>
#include <seqan/bam_io.h>
#include <seqan/sequence.h>






class BamViewer : public QAbstractScrollArea
{
public:
    BamViewer(QWidget * parent = nullptr);

    void setReferenceFile(const QString& fastaFile);
    void setAlignementFile(const QString& bamFile);
    void setRegion(const QString& chr, quint64 start, quint64 end);



    const QString &referenceFile() const;
    const QString &alignementFile() const;

    quint64 idFromChromosom(const seqan::CharString& chromosom) const;

protected:

    // -- geometry
    void updateScrollBar();
    void scrollContentsBy(int dx, int dy) override;
    void resizeEvent(QResizeEvent * event) override;


    // -- paint session
    void paintEvent(QPaintEvent * event)  override;
    void paintReference(QPainter& painter);


    // -- utils session

    QString referenceIndexFile() const;
    quint64 currentReferenceSize() const;

    seqan::Dna5String currentReferenceSequence();




private:
    QString mReferenceFile;
    QString mAlignementFile;
    seqan::GenomicRegion mRegion;
    seqan::FaiIndex mFaiIndex;


};

#endif // BAMVIEWER_H
