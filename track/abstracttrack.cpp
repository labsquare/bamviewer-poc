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

QString AbstractTrack::name() const
{
    return mName;
}

void AbstractTrack::setName(const QString &name)
{
    mName = name;
}


