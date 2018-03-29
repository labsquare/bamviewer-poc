#ifndef OLIVIERTRACK_H
#define OLIVIERTRACK_H
#include "abstracttrack.h"

class OlivierTrack : public AbstractTrack
{
public:
    OlivierTrack();


    virtual void paint(QPainter * painter, seqan::GenomicRegion& region, int width) final;
    virtual int height() final;


};

#endif // OLIVIERTRACK_H
