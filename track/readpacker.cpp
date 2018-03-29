#include "readpacker.h"

ReadPacker::ReadPacker()
{

}
int ReadPacker::getYRecord(const seqan::BamAlignmentRecord &record)
{
    int rowIndex = -1;


    qDebug()<<mOliveData.size();

    QByteArray key(seqan::length(record.qName), 'A');


    std::copy(seqan::begin(record.qName),
              seqan::end(record.qName),
              key.begin());



    if (mOliveData.contains(key))
        return mOliveData[key];


    for (auto it = mRows.begin(); it != mRows.end(); it++)
    {
        if (record.beginPos > it->second)
        {
            it->second = record.beginPos + seqan::length(record.seq);
            rowIndex = it - mRows.begin();
            break;
        }

        else if (record.beginPos + seqan::length(record.seq) < it->first)
        {
            it->first = record.beginPos;
            rowIndex = it - mRows.begin();
            break;
        }

    }

    // create new row
    if ( rowIndex == -1)
        rowIndex = mRows.length();

    RecordRow row = qMakePair(record.beginPos, record.beginPos + seqan::length(record.seq));
    mRows.append(row);

    mOliveData[key] = rowIndex;

    return rowIndex;
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
