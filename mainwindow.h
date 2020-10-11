#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QFont>

#include <qcustomplot.h>
#include "dep/TEClib/dateTime.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void make_plot(double,
                    QVector<QPair<QVector<double>, QVector<double>>>&,
                   QVector<QPair<QVector<double>, QVector<double>>>&,
                   QVector<QPair<QVector<double>, QVector<double>>>&,
                   QVector<QPair<QVector<double>, QVector<double>>>&);
private:
    Ui::MainWindow *ui;
    QCustomPlot *wGraphic;
};
#endif // MAINWINDOW_H
