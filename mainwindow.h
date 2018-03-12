#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "bamviewer.h"
#include <QContiguousCache>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    void updateRegion();


private:
    BamViewer * mViewer;

    QLineEdit * mEdit;
    QSlider * mSlider;
};

#endif // MAINWINDOW_H
