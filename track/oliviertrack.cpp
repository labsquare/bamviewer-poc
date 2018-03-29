#include "oliviertrack.h"

OlivierTrack::OlivierTrack()
{

}

void OlivierTrack::paint(QPainter *painter, seqan::GenomicRegion &region, int width)
{

    painter->drawText(viewer()->viewport()->rect(),
                      Qt::AlignCenter|Qt::AlignVCenter,
                      QString("%1 %2").arg(region.beginPos).arg(region.endPos));
}

int OlivierTrack::height()
{
    return 10 ;
}
