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
                           QVector<QPair<QVector<double>, QVector<double>>>& tecCRForPlot,
                           QVector<QPair<QVector<double>, QVector<double>>>& tecLRForPlot,
                           QVector<QPair<QVector<double>, QVector<double>>>& tecCGForPlot,
                           QVector<QPair<QVector<double>, QVector<double>>>& tecLGForPlot)
{
    wGraphic = new QCustomPlot();
    ui->gridLayout->addWidget(wGraphic,1,0,1,1);
    wGraphic->legend->setVisible(true);
    int i = 0;
    for(auto &tecCRForPoints:tecCRForPlot){
        if(tecCRForPoints.first.size()>0){
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecCRForPoints.first,tecCRForPoints.second);
            wGraphic->graph(i)->setPen(QColor(250, 0, 50, 255));//задаем цвет точки
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);//убираем линии
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecCR " + QString::number(i));

            i++;
        }
    }
    for(auto &tecLRForPoints:tecLRForPlot){
        if(tecLRForPoints.first.size()>0){
       //     qDebug()<<tecLRForPoints.first.size();
         //   qDebug()<<tecLRForPoints.second.size();
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecLRForPoints.first,tecLRForPoints.second);
            wGraphic->graph(i)->setPen(QColor(25, 0, 50, 255));//задаем цвет точки
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);//убираем линии
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecLR " + QString::number(i));
            i++;
        }
    }
    for(auto &tecCGForPoints:tecCGForPlot){
        if(tecCGForPoints.first.size()>0){
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecCGForPoints.first,tecCGForPoints.second);
            wGraphic->graph(i)->setPen(QColor(25, 0, 50, 255));//задаем цвет точки
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);//убираем линии
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecCG " + QString::number(i));
            i++;
        }
    }
    for(auto &tecLGForPoints:tecLGForPlot){
        if(tecLGForPoints.first.size()>0){
            wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
            wGraphic->graph(i)->setData(tecLGForPoints.first,tecLGForPoints.second);
            wGraphic->graph(i)->setPen(QColor(25, 0, 50, 255));//задаем цвет точки
            wGraphic->graph(i)->setLineStyle(QCPGraph::lsNone);//убираем линии
            wGraphic->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 0.5));
            wGraphic->graph(i)->setName("tecLG " + QString::number(i));
            i++;
        }
    }

    wGraphic->xAxis->setLabel("time");
    wGraphic->yAxis->setLabel("tec");

    wGraphic->xAxis->setRange(startMoment,startMoment+86400);
    wGraphic->yAxis->setRange(-50,100);

    wGraphic->replot();
}
