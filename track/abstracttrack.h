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

    virtual int height() = 0;


    QString name() const;
    void setName(const QString &name);

private:
    TrackViewer * mViewer;
    QString mName;


};

#endif // ABSTRACTTRACK_H
