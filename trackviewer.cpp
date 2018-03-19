#include "trackviewer.h"

#include "referencetrack.h"

TrackViewer::TrackViewer(QWidget *parent)
    :QAbstractScrollArea(parent)
{

}

void TrackViewer::setRegion(const QString &chr, int start, int end)
{
    seqan::GenomicRegion region;

    region.seqName  = chr.toStdString();
    region.beginPos = start;
    region.endPos   = end;

    setRegion(region);
}

void TrackViewer::setRegion(const seqan::GenomicRegion &region)
{
    mRegion = region;
    updateScrollBar();
    viewport()->update();
}

void TrackViewer::addTrack(AbstractTrack *track)
{
    track->setViewer(this);
    mTracks.append(track);

}

const seqan::GenomicRegion &TrackViewer::region() const
{
    return mRegion;
}

int TrackViewer::regionLength() const
{
    return mRegion.endPos - mRegion.beginPos;
}

void TrackViewer::setReference(const QString &filename)
{
    if (!mReferenceTrack){
        mReferenceTrack = new ReferenceTrack();
        addTrack(mReferenceTrack);
    }

    mReferenceTrack->setFilename(filename);
}

ReferenceTrack *TrackViewer::referenceTrack() const
{
    return mReferenceTrack;
}

void TrackViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    painter.setFont(font);

    for (AbstractTrack * track : mTracks)
    {
        track->paint(&painter, mRegion, viewport()->width());
        painter.translate(0, mTrackSpacing);
    }
}


void TrackViewer::updateScrollBar()
{
    int min = 1 ;
    int max = mReferenceTrack->baseCount() -  viewport()->width();

    qDebug()<<"update scroll bar "<<min<<" "<<max;


    horizontalScrollBar()->setRange(min , max );
    horizontalScrollBar()->setPageStep(viewport()->width());

}

void TrackViewer::scrollContentsBy(int dx, int dy)
{
    mRegion.beginPos += -dx ;
    mRegion.endPos   += -dx;
    viewport()->update();
}
