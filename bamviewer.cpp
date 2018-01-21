#include "bamviewer.h"

BamViewer::BamViewer(QObject *parent)
{


}

void BamViewer::setFilename(const QString &filename)
{
    QFileInfo info(filename);
    mFilename = info.absoluteFilePath();

    QString s = "/home/sacha/exemple.bam";
    qDebug()<<s<<" "<<QFile::exists(s);

    BamFileIn file(s.toStdString().c_str());


    BamHeader header;
    readHeader(header,file);

   while (!atEnd(file))
   {
       BamAlignmentRecord record;

       readRecord(record, file);

       qDebug()<<record.beginPos;


   }





}

void BamViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(viewport());



}
