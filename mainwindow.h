#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bamviewer.h"
#include <QContiguousCache>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    BamViewer * mViewer;


};

#endif // MAINWINDOW_H
