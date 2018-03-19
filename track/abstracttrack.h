#ifndef ABSTRACTTRACK_H
#define ABSTRACTTRACK_H
#include <QtCore>
#include <QPainter>
#include "trackviewer.h"

class TrackViewer;
class AbstractTrack;

class AbstractTrack
{

public:
    explicit AbstractTrack();
    virtual ~AbstractTrack();


    TrackViewer * viewer() const;
    void setViewer(TrackViewer * viewer);

    virtual void paint(QPainter * painter, seqan::GenomicRegion& region, int width) = 0;


private:
    TrackViewer * mViewer;


};

#endif // ABSTRACTTRACK_H
