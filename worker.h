#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include <QThread>

#include <fstream>
#include <string>
#include "mainwindow.h"
#include "NFileParser.h"
#include "GFileParser.h"
#include "OFileParser.h"
#include "IFileParser.h"
#include "models.h"
#include <algorithm>
#include "Tec.h"

Q_DECLARE_METATYPE(std::vector<OFileData>)

class SubWorkerR: public QObject
{
    Q_OBJECT
public slots:
    void doWork(std::vector<OFileData>, std::vector<std::string>);
signals:
    void resultReady(std::vector<TECvalR>);
};

class SubWorkerG: public QObject
{
    Q_OBJECT
public slots:
    void doWork(std::vector<OFileData>, std::vector<std::string>);
signals:
    void resultReady(std::vector<TECvalG>);
};

class Worker: public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();
public slots:
    void doWork(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);
    void handleResultR(std::vector<TECvalR>);
    void handleResultG(std::vector<TECvalG>);
signals:
    void operateR(std::vector<OFileData>, std::vector<std::string>);
    void operateG(std::vector<OFileData>, std::vector<std::string>);
    void resultReady(std::vector<TECvalR>, std::vector<TECvalG>);
private:
    QThread workerThreadR;
    QThread workerThreadG;
    std::vector<TECvalR> tecR;
    std::vector<TECvalG> tecG;
};

#endif // WORKER_H
