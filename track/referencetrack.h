#ifndef REFERENCETRACK_H
#define REFERENCETRACK_H

#include "abstracttrack.h"

#include <seqan/seq_io.h>
#include <seqan/bam_io.h>
#include <seqan/sequence.h>

#include <istream>

class TrackViewer;
class ReferenceTrack;
class ReferenceTrack : public AbstractTrack
{
  public:
    ReferenceTrack(const QString& filename = QString());

    /*!
     * \brief set Reference fasta file.
     * You need a fai index file
     * \param filename
     */
    void setFilename(const QString& filename);

    quint64 baseCount(const seqan::CharString& chromosom) const;
    quint64 baseCount() const;

    quint64 chromosomToId(const seqan::CharString& chromosom) const;

    QStringList chromosoms() const;


    seqan::Dna5String sequence(const seqan::GenomicRegion &region);
    seqan::Dna5String sequence();

    void paint(QPainter * painter, seqan::GenomicRegion& region, int width) override;
    int height() override;


private:
    QString mFastaFile;
    seqan::FaiIndex mFaiIndex;


};



#endif // REFERENCETRACK_H
