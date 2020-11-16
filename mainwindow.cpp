#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::make_plot(double startMoment,
                           double endMoment,
                           QVector<QPair<QVector<double>, QVector<double>>>& tecCRForPlot,
                           QVector<QPair<QVector<double>, QVector<double>>>& tecLRForPlot,
                           QVector<QPair<QVector<double>, QVector<double>>>& tecCGForPlot,
                           QVector<QPair<QVector<double>, QVector<double>>>& tecLGForPlot)
{
    wGraphic = new QCustomPlot();
    ui->gridLayout->addWidget(wGraphic,1,0,1,1);
    wGraphic->legend->setVisible(true);
    //wGraphic->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
    int i = 0;
    int satNumber=1;
    for(auto &tecCRForPoints:tecCRForPlot){
        if(tecCRForPoints.first.size()>0){
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecCRForPoints.first,tecCRForPoints.second);
            wGraphic->graph(i)->setPen(QColor(250, 0, 50, 255));
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecCGLONASS " + QString::number(satNumber));

            i++;
        }
        satNumber++;
    }

    satNumber=1;
    for(auto &tecLRForPoints:tecLRForPlot){
        if(tecLRForPoints.first.size()>0){
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecLRForPoints.first,tecLRForPoints.second);
            wGraphic->graph(i)->setPen(QColor(25, 0, 50, 255));
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecLGLONASS " + QString::number(satNumber));
            i++;
        }
        satNumber++;
    }

    satNumber=1;
    for(auto &tecCGForPoints:tecCGForPlot){
        if(tecCGForPoints.first.size()>0){
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecCGForPoints.first,tecCGForPoints.second);
            wGraphic->graph(i)->setPen(QColor(0, 150, 0, 255));//задаем цвет точки
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);//убираем линии
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecCGPS " + QString::number(satNumber));
            i++;
        }
        satNumber++;
    }

    satNumber=1;
    for(auto &tecLGForPoints:tecLGForPlot){
        if(tecLGForPoints.first.size()>0){
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecLGForPoints.first,tecLGForPoints.second);
            wGraphic->graph(i)->setPen(QColor(0, 175, 150, 255));//задаем цвет точки
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);//убираем линии
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecLGPS " + QString::number(satNumber));
            i++;
        }
        satNumber++;
    }

    wGraphic->xAxis->setLabel("time");
    wGraphic->yAxis->setLabel("tec");

    wGraphic->xAxis->setRange(startMoment, endMoment);
    wGraphic->yAxis->setRange(-20,100);

    wGraphic->replot();
}
