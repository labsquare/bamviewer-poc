#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QContiguousCache>
#include "trackviewer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:


    QSlider * mSlider;
    QLineEdit * mEdit;
    TrackViewer * mViewer;

};

#endif // MAINWINDOW_H
