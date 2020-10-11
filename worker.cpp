#include "worker.h"

void SubWorkerR::doWork(std::vector<OFileData> oFileData,
                        std::vector<IFileData> iFileData,
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

    auto TECR = TEC(rSatPositions, point);

    TECR = resolveBIAS(TECR, rSatPositions, point, iFileData);

    TECR = resolvePhaseAmbiguity(TECR);

    emit this->resultReady(TECR);
}

void SubWorkerG::doWork(std::vector<OFileData> oFileData,
                        std::vector<IFileData> iFileData,
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

    auto TECG = TEC(gSatPositions, point);

    TECG = resolveBIAS(TECG, gSatPositions, point, iFileData);

    TECG = resolvePhaseAmbiguity(TECG);

    emit this->resultReady(TECG);
}

Worker::Worker(){

    qRegisterMetaType<std::vector<OFileData> >("std::vector<OFileData>");
    qRegisterMetaType<std::vector<IFileData> >("std::vector<IFileData>");
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
                    std::vector<std::string> oFileNames,
                    std::vector<std::string> iFileNames){
    std::ifstream *file = new std::ifstream;

    std::vector<OFileData> oFileData;

    for (auto name: oFileNames){
        file->open(name);
        auto _oFileData = parseOFile(file);
        oFileData.insert(oFileData.end(), _oFileData.begin(), _oFileData.end());
        file->close();
    }

    std::vector<IFileData> iFileData;

    for (auto name: iFileNames){
        file->open(name);
        auto _iFileData = parseIFile(file);
        iFileData.insert(iFileData.end(), _iFileData.begin(), _iFileData.end());
        file->close();
    }

    emit this->operateR(oFileData, iFileData, gFileNames);
    emit this->operateG(oFileData, iFileData, nFileNames);
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
