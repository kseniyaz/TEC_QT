#include <fstream>
#include "opening_window.h"
#include "mainwindow.h"
#include "NFileParser.h"
#include "GFileParser.h"
#include "OFileParser.h"
#include "IFileParser.h"
#include "models.h"
#include <algorithm>
#include "Tec.h"

#include <QThread>
#include <QVector>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    opening_window o;
    o.show();

    return a.exec();
}
