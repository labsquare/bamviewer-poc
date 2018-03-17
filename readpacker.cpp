#include "readpacker.h"

ReadPacker::ReadPacker()
{

}

void ReadPacker::addRecord(const seqan::BamAlignmentRecord &record)
{
    for (auto& row : mRows)
    {
        seqan::BamAlignmentRecord rec = row.last();
        if (int(record.beginPos  > int(rec.beginPos + seqan::length(rec.seq))))
        {
            row.append(record);
            return;
        }
    }

    // create new row
    RecordRow row;
    row.append(record);
    mRows.append(row);
}

void ReadPacker::clear()
{
    for (auto i : mRows)
        i.clear();

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
