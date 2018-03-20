#include "trackviewer.h"

#include "referencetrack.h"

TrackViewer::TrackViewer(QWidget *parent)
    :QAbstractScrollArea(parent)
{
    mRegion.seqName  = "";
    mRegion.beginPos = 1;
    mRegion.endPos   = 100;
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

    emit regionChanged(regionString());


}

void TrackViewer::addTrack(AbstractTrack *track)
{
    if (track != mReferenceTrack && mReferenceTrack == nullptr)
    {
        qWarning()<<"WARNING,  cannot add track before setting reference track";
        return;
    }

    track->setViewer(this);
    mTracks.append(track);

    updateScrollBar();

}

const seqan::GenomicRegion &TrackViewer::region() const
{
    return mRegion;
}

QString TrackViewer::regionString() const
{
    std::string string;
    mRegion.toString<std::string>(string);

    return QString::fromStdString(string);
}

int TrackViewer::regionLength() const
{
    return mRegion.endPos - mRegion.beginPos;
}

void TrackViewer::setReference(const QString &filename)
{
    if (!mReferenceTrack){
        mReferenceTrack = new ReferenceTrack(filename);
        addTrack(mReferenceTrack);
    }
    else
        mReferenceTrack->setFilename(filename);

}

ReferenceTrack *TrackViewer::referenceTrack() const
{
    return mReferenceTrack;
}

AbstractTrack *TrackViewer::track(int index)
{
    return mTracks[index];
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

    // set horizontal
    int min = 1 ;

    qDebug()<<mReferenceTrack->baseCount();

    int max = mReferenceTrack->baseCount() - regionLength();
    horizontalScrollBar()->setRange(min , max );
    horizontalScrollBar()->setPageStep(viewport()->width());

    // set vetical
    int totalHeight = 0;
    for (auto * i : mTracks)
        totalHeight+= i->height();

    verticalScrollBar()->setRange(1 , totalHeight );

}

void TrackViewer::scrollContentsBy(int dx, int dy)
{
    setRegion(seqan::toCString(region().seqName), mRegion.beginPos - dx , mRegion.endPos - dx);
}
