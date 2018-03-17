#ifndef READPACKER_H
#define READPACKER_H
#include <QtCore>
#include <seqan/bam_io.h>



class ReadPacker
{
public:

    typedef QList<seqan::BamAlignmentRecord> RecordRow;
    ReadPacker();

    void addRecord(const seqan::BamAlignmentRecord& record);
    void clear() ;
    int rows() const;

    const RecordRow& record(int row) const;


private:
    QList<RecordRow> mRows;


};

#endif // READPACKER_H
