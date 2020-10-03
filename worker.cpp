#include "worker.h"

void SubWorkerR::doWork(std::vector<OFileData> oFileData,
                        std::vector<std::string> gFileNames){
    std::ifstream *file = new std::ifstream;

    std::vector<GFileData> gFileData;

    for (auto name: gFileNames){
        file->open(name);
        auto _gFileData = parseGFile(file);
        gFileData.insert(gFileData.end(), _gFileData.begin(), _gFileData.end());
        file->close();
    }

    auto rSatPositions = RSatPositions(oFileData, gFileData);

    std::array<double, 3> point{-370700.2945,  1225384.5150, -6230930.2438};

    auto TECR = TEC<TECvalR>(rSatPositions, point);

    for(std::size_t index = 0; index < TECR[0].sat.size(); index++){
        double diff = 0;
        int count = 0, list = 0;
        std::size_t i1;
        for(std::size_t i = 0;i <TECR.size(); i++){
            if(!isnan(TECR[i].sat[index].tecC) && !isnan(TECR[i].sat[index].tecL))
             {
                if(count == 0)   {
                    i1=i;
                }
                  diff += (TECR[i].sat[index].tecL - TECR[i].sat[index].tecC);
                  count ++;
                  list = 0;
             }
            else
                list++;
            if(list > 10 && count > 1)
            {
                for(;i1<i;i1++){
                    if(!isnan(TECR[i1].sat[index].tecL))
                    {
                        TECR[i1].sat[index].tecL -= diff/count;
                    }
                }

                list = 0;
                diff = 0;
                count = 0;
            }
        }
    }
    emit this->resultReady(TECR);
}

void SubWorkerG::doWork(std::vector<OFileData> oFileData,
                        std::vector<std::string> nFileNames){
    std::ifstream *file = new std::ifstream;

    std::vector<NFileData> nFileData;

    for (auto name: nFileNames){
        file->open(name);
        auto _nFileData = parseNFile(file);
        nFileData.insert(nFileData.end(), _nFileData.begin(), _nFileData.end());
        file->close();
    }

    auto gSatPositions = GSatPositions(oFileData, nFileData);

    std::array<double, 3> point{-370700.2945,  1225384.5150, -6230930.2438};

    auto TECG = TEC<TECvalG>(gSatPositions, point);

    for(std::size_t index = 0; index < TECG[0].sat.size(); index++){
        double diff = 0;
        int count = 0, list = 0;
        std::size_t i1;
        for(std::size_t i = 0;i <TECG.size(); i++){
            if(!isnan(TECG[i].sat[index].tecC) && !isnan(TECG[i].sat[index].tecL))
             {
                if(count == 0)   {
                    i1=i;
                }
                  diff += (TECG[i].sat[index].tecL - TECG[i].sat[index].tecC);
                  count ++;
                  list = 0;
             }
            else
                list++;
            if(list > 10 && count > 1)
            {
                for(;i1<i;i1++){
                    if(!isnan(TECG[i1].sat[index].tecL))
                    {
                        TECG[i1].sat[index].tecL -= diff/count;
                    }
                }
                //qDebug()<<diff/count;
                list = 0;
                diff = 0;
                count = 0;
            }
        }
    }
    emit this->resultReady(TECG);
}

Worker::Worker(){

    qRegisterMetaType<std::vector<OFileData> >("std::vector<OFileData>");
    SubWorkerR *workerR = new SubWorkerR;
    workerR->moveToThread(&workerThreadR);

    connect(&workerThreadR, &QThread::finished, workerR, &QObject::deleteLater);
    connect(this, &Worker::operateR, workerR, &SubWorkerR::doWork);
    connect(workerR, &SubWorkerR::resultReady, this, &Worker::handleResultR);

    workerThreadR.start();

    SubWorkerG *workerG = new SubWorkerG;
    workerG->moveToThread(&workerThreadG);

    connect(&workerThreadG, &QThread::finished, workerG, &QObject::deleteLater);
    connect(this, &Worker::operateG, workerG, &SubWorkerG::doWork);
    connect(workerG, &SubWorkerG::resultReady, this, &Worker::handleResultG);

    workerThreadG.start();
}

Worker:: ~Worker(){
    workerThreadR.quit();
    workerThreadR.wait();

    workerThreadG.quit();
    workerThreadG.wait();
}


void Worker::doWork(std::vector<std::string> gFileNames,
                    std::vector<std::string> nFileNames,
                    std::vector<std::string> oFileNames){
    std::ifstream *file = new std::ifstream;

    std::vector<OFileData> oFileData;

    for (auto name: oFileNames){
        file->open(name);
        auto _oFileData = parseOFile(file);
        oFileData.insert(oFileData.end(), _oFileData.begin(), _oFileData.end());
        file->close();
    }

    emit this->operateR(oFileData, gFileNames);
    emit this->operateG(oFileData, nFileNames);
}

void Worker::handleResultG(std::vector<TECvalG> TECG){
    tecG = TECG;
    if(tecR.size()>0){
        emit this->resultReady(tecR, tecG);
    }
}

void Worker::handleResultR(std::vector<TECvalR> TECR){
    tecR = TECR;
    if(tecG.size()>0){
        emit this->resultReady(tecR, tecG);
    }
}
