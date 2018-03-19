#include "readpacker.h"

ReadPacker::ReadPacker()
{

}

int ReadPacker::getYRecord(const seqan::BamAlignmentRecord &record)
{
    for (auto it = mRows.begin(); it != mRows.end(); it++)
    {
        if (record.beginPos + seqan::length(record.seq) < it->first)
        {
            it->first = record.beginPos;
            return it - mRows.begin();
        }
        else if (record.beginPos > it->second)
        {
            it->second = record.beginPos + seqan::length(record.seq);
            return it - mRows.begin();
        }
    }

    // create new row
    RecordRow row = qMakePair(record.beginPos, record.beginPos + seqan::length(record.seq));
    mRows.append(row);
    return mRows.length();
}

void ReadPacker::clear()
{
    mRows.clear();
}

int ReadPacker::rows() const
{
    return mRows.size();
}

const ReadPacker::RecordRow &ReadPacker::record(int row) const
{
    return mRows.at(row);
}
