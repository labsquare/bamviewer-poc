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

Q_OBJECT
public:
    BamViewer(QWidget * parent = nullptr);

    /*!
     * \brief set fasta reference genom
     * \param fastaFile
     * \details ex: setReferenceFile("hg19.fa")
     */
    void setReferenceFile(const QString& fastaFile);

    /*!
     * \brief set Alignement BAM/Sam File
     * \param bamFile
     * \details setAlignementFile("sampole1.bam")
     */
    void setAlignementFile(const QString& bamFile);

    /*!
     * \brief set Region to display
     * \param chr
     * \param start
     * \param end
     */
    void setRegion(const QString& chr, int start, int  end);

    /*!
     * \brief region
     * \return current Genomic Region
     */
    const seqan::GenomicRegion &region() const;

    QString regionName() const;


    const QString &referenceFile() const;
    const QString &alignementFile() const;



   Q_SIGNALS:
    void regionChanged();


protected:

    quint64 idFromChromosom(const seqan::CharString& chromosom) const;


    // -- geometry
    void updateScrollBar();
    void scrollContentsBy(int dx, int dy) override;
    void resizeEvent(QResizeEvent * event) override;


    // -- paint session
    void paintEvent(QPaintEvent * event)  override;
    void paintReference(QPainter& painter);
    void paintAlignement(QPainter& painter);



    // -- utils session

    QString referenceIndexFile() const;
    quint64 referenceLength(const QString& referenceName = QString()) const;

    seqan::Dna5String regionSequence() const;




private:
    QString mReferenceFile;
    QString mAlignementFile;
    seqan::GenomicRegion mRegion;
    seqan::FaiIndex mFaiIndex;


};

#endif // BAMVIEWER_H
