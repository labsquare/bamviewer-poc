#ifndef BAMVIEWER_H
#define BAMVIEWER_H
#include <QAbstractScrollArea>
#include <QPainter>
#include <QtCore>
#include <zlib.h>

#include "seqan/bam_io.h"
#include <iostream>

#define SEQAN_HAS_ZLIB 1

using namespace seqan;

class BamViewer : public QAbstractScrollArea
{
public:
    BamViewer(QObject * parent = nullptr);

    void setFilename(const QString& filename);

protected:

    void paintEvent(QPaintEvent * event)  override;

private:
    QString mFilename;

};

#endif // BAMVIEWER_H
