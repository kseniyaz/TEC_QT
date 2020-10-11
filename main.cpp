#include "opening_window.h"
#include "mainwindow.h"

#include <fstream>
#include <algorithm>

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

#include "dep/TEClib/NFileParser.h"
#include "dep/TEClib/GFileParser.h"
#include "dep/TEClib/OFileParser.h"
#include "dep/TEClib/IFileParser.h"
#include "dep/TEClib/models.h"
#include "dep/TEClib/Tec.h"

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    opening_window o;
    o.show();

    return a.exec();
}
