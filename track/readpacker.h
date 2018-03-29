#ifndef READPACKER_H
#define READPACKER_H
#include <QtCore>
#include <seqan/bam_io.h>



class ReadPacker
{
public:

    typedef QPair<std::uint64_t, std::uint64_t> RecordRow;

    // RecordRow ( start, end )

    ReadPacker();

    int getYRecord(const seqan::BamAlignmentRecord& record);
    void clear() ;
    int rows() const;

    const RecordRow& record(int row) const;


private:
    QList<RecordRow> mRows;
    QHash<QByteArray, int> mOliveData;  // record name : row index




};

#endif // READPACKER_H
