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

    /*!
     * \brief setReferenceFile
     * genom fasta file with index
     * ex: ecoli.fa / ecoli.fa.fai
     * \param fastaFile
     */
    void setReferenceFile(const QString& fastaFile);
    /*!
     * \brief setAlignementFile
     * aligment file as BAM format with index
     * ex: sample1.bam / sample1.bam.bai
     * \param bamFile
     */
    void setAlignementFile(const QString& bamFile);
    /*!
     * \brief setRegion to display
     * \param chr : the name of the sequence to use from reference
     * \param start: start coordinate ( 1-first )
     * \param end : end coordinaite
     */
    void setRegion(const QString& chr, int start, int end);
    void setRegion(const seqan::GenomicRegion& region);


    /*!
     * \brief regionLength
     * region.end - region.start
     * \return length in nucleotid
     */
    int regionLength() const;

    /*!
     * \brief referenceLength
     * return reference length of a specific chromosom
     * \param chr
     * \return length in nucleotid
     */
    quint64 referenceLength(const QString& chr) const;

    /*!
     * \brief currentReferenceLength
     * return referenceLength of the current region chromosom
     * \return length in nucleotid
     */
    quint64 currentReferenceLength() const;


    /*!
     * \brief referenceSequence
     * Extract sequence from reference and a specific region
     * \param region
     * \return nucleotid sequence in Dna5String
     */
    seqan::Dna5String referenceSequence(const seqan::GenomicRegion& region);

    /*!
     * \brief currentReferenceSequence
     * Extract sequence from reference and the current region
     * \param region
     * \return nucleotid sequence in Dna5String
     */
    seqan::Dna5String currentReferenceSequence();


    const QString &referenceFile() const;
    const QString &alignementFile() const;


protected:

    // -- geometry
    // update scrollbar ( min, max, page step .. )
    void updateScrollBar();

    // this method is called when the scroll bar moved
    void scrollContentsBy(int dx, int dy) override;

    // unused now : called when widget resize
    void resizeEvent(QResizeEvent * event) override;


    // -- paint session

    void paintEvent(QPaintEvent * event)  override;

    // paint reference sequence
    void paintReference(QPainter& painter);

    // paint alignement sequence
    void paintAlignement(QPainter& painter);





    // -- utils session

    /*!
     * \brief idFromChromosom
     * return numerical ID from chromsom name used by seqan
     * ex: ecoli ==> 1
     * \param chromosom
     * \return
     */
    quint64 idFromChromosom(const seqan::CharString& chromosom) const;




private:
    QString mReferenceFile;
    QString mAlignementFile;
    seqan::GenomicRegion mRegion;
    seqan::FaiIndex mFaiIndex;



};

#endif // BAMVIEWER_H
