#ifndef TRACKVIEWER_H
#define TRACKVIEWER_H

#include <QtWidgets>
#include <QPainter>
#include <seqan/seq_io.h>
#include <seqan/bam_io.h>
#include <seqan/sequence.h>

#include "abstracttrack.h"

class TrackViewer;
class AbstractTrack;
class ReferenceTrack;

class TrackViewer :public QAbstractScrollArea
{
    Q_OBJECT
public:
    TrackViewer(QWidget * parent = 0);

    /*!
     * \brief setRegion to display
     * \param chr : the name of the sequence to use from reference
     * \param start: start coordinate ( 1-first )
     * \param end : end coordinaite
     */
    void setRegion(const QString& chr, int start, int end);
    void setRegion(const seqan::GenomicRegion& region);

    void addTrack(AbstractTrack * track);

    const seqan::GenomicRegion& region() const;
    int regionLength() const;

    void setReference(const QString& filename);

    ReferenceTrack * referenceTrack() const;

protected:

    void paintEvent(QPaintEvent * event) override;

    // -- geometry
    // update scrollbar ( min, max, page step .. )
    void updateScrollBar();

    // this method is called when the scroll bar moved
    void scrollContentsBy(int dx, int dy) override;

private:
    seqan::GenomicRegion mRegion;
    QList<AbstractTrack*> mTracks;


    int mTrackSpacing = 40;

    // mandatory track
    ReferenceTrack * mReferenceTrack = nullptr;



};

#endif // TRACKVIEWER_H
