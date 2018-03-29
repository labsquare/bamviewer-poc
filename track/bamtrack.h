#ifndef BAMTRACK_H
#define BAMTRACK_H
#include "abstracttrack.h"
#include <seqan/seq_io.h>
#include <seqan/bam_io.h>
#include <seqan/sequence.h>

#include "readpacker.h"

class BamTrack : public AbstractTrack
{
public:
    BamTrack(const QString filename = QString());

    void setFilename(const QString& filename);
    virtual void paint(QPainter * painter, seqan::GenomicRegion& region, int width) override;
    virtual int height() override;


protected:
    void addRecordToDepth(const seqan::BamAlignmentRecord& record);


private:
    QString mFilename;

    seqan::BamFileIn mBamFileIn;
    seqan::BamIndex<seqan::Bai> mBaiIndex;
    seqan::BamHeader mHeader;

    ReadPacker mReadPacker;

    QVector<int> mDepths;
    int mMaxDepth;

    int mRegionMargin = 100;

    seqan::GenomicRegion oldRegion;

};

#endif // BAMTRACK_H
