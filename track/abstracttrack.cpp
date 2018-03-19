#include "abstracttrack.h"


AbstractTrack::AbstractTrack()
{

}

AbstractTrack::~AbstractTrack()
{

}

TrackViewer *AbstractTrack::viewer() const
{
    return mViewer;
}

void AbstractTrack::setViewer(TrackViewer *viewer)
{
    mViewer = viewer;
}


