#include "opening_window.h"
#include "ui_opening_window.h"

#include<vector>

#include <QPair>

opening_window::opening_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::opening_window)
{
     ui->setupUi(this);
     ui->stackedWidget->setCurrentIndex(0);
     qRegisterMetaType<std::vector<std::string> >("std::vector<std::string>");
     qRegisterMetaType<QVector<double> >("QVector<double>");
     qRegisterMetaType<std::vector<TECvalG>>("std::vector<TECvalG>");
     qRegisterMetaType<std::vector<TECvalR>>("std::vector<TECvalR>");

     Worker *worker = new Worker;
     worker->moveToThread(&workerThread);

     connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
     connect(this, &opening_window::operate, worker, &Worker::doWork);
     connect(worker, &Worker::resultReady, this, &opening_window::handleResult);

     workerThread.start();

     gCheckBoxes.append(ui->checkBox_G1);
     gCheckBoxes.append(ui->checkBox_G2);
     gCheckBoxes.append(ui->checkBox_G3);
     gCheckBoxes.append(ui->checkBox_G4);
     gCheckBoxes.append(ui->checkBox_G5);
     gCheckBoxes.append(ui->checkBox_G6);
     gCheckBoxes.append(ui->checkBox_G7);
     gCheckBoxes.append(ui->checkBox_G8);
     gCheckBoxes.append(ui->checkBox_G9);
     gCheckBoxes.append(ui->checkBox_G10);
     gCheckBoxes.append(ui->checkBox_G11);
     gCheckBoxes.append(ui->checkBox_G12);
     gCheckBoxes.append(ui->checkBox_G13);
     gCheckBoxes.append(ui->checkBox_G14);
     gCheckBoxes.append(ui->checkBox_G15);
     gCheckBoxes.append(ui->checkBox_G16);
     gCheckBoxes.append(ui->checkBox_G17);
     gCheckBoxes.append(ui->checkBox_G18);
     gCheckBoxes.append(ui->checkBox_G19);
     gCheckBoxes.append(ui->checkBox_G20);
     gCheckBoxes.append(ui->checkBox_G21);
     gCheckBoxes.append(ui->checkBox_G22);
     gCheckBoxes.append(ui->checkBox_G23);
     gCheckBoxes.append(ui->checkBox_G24);
     gCheckBoxes.append(ui->checkBox_G25);
     gCheckBoxes.append(ui->checkBox_G26);
     gCheckBoxes.append(ui->checkBox_G27);
     gCheckBoxes.append(ui->checkBox_G28);
     gCheckBoxes.append(ui->checkBox_G29);
     gCheckBoxes.append(ui->checkBox_G30);
     gCheckBoxes.append(ui->checkBox_G31);
     gCheckBoxes.append(ui->checkBox_G32);

     rCheckBoxes.append(ui->checkBox_R1);
     rCheckBoxes.append(ui->checkBox_R2);
     rCheckBoxes.append(ui->checkBox_R3);
     rCheckBoxes.append(ui->checkBox_R4);
     rCheckBoxes.append(ui->checkBox_R5);
     rCheckBoxes.append(ui->checkBox_R6);
     rCheckBoxes.append(ui->checkBox_R7);
     rCheckBoxes.append(ui->checkBox_R8);
     rCheckBoxes.append(ui->checkBox_R9);
     rCheckBoxes.append(ui->checkBox_R10);
     rCheckBoxes.append(ui->checkBox_R11);
     rCheckBoxes.append(ui->checkBox_R12);
     rCheckBoxes.append(ui->checkBox_R13);
     rCheckBoxes.append(ui->checkBox_R14);
     rCheckBoxes.append(ui->checkBox_R15);
     rCheckBoxes.append(ui->checkBox_R16);
     rCheckBoxes.append(ui->checkBox_R17);
     rCheckBoxes.append(ui->checkBox_R18);
     rCheckBoxes.append(ui->checkBox_R19);
     rCheckBoxes.append(ui->checkBox_R20);
     rCheckBoxes.append(ui->checkBox_R21);
     rCheckBoxes.append(ui->checkBox_R22);
     rCheckBoxes.append(ui->checkBox_R23);
     rCheckBoxes.append(ui->checkBox_R24);
}

opening_window::~opening_window()
{
    delete ui;
    workerThread.quit();
    workerThread.wait();
}

void opening_window::on_choose_G_Button_clicked()
{
     QStringList list = QFileDialog::getOpenFileNames(0, "Open Dialog", "", "*.1?g");
     std::vector<std::string> gFileNames ;

      for(auto elem : list){

          gFileNames.push_back(elem.toStdString());
          return_g_names.push_back(elem.toStdString());
          ui->g_lineEdit->insert((elem) + "...");
    }
}
void opening_window::on_choose_N_Button_clicked()
{
    QStringList list = QFileDialog::getOpenFileNames(0, "Open Dialog", "", "*.1?n");
    std::vector<std::string> nFileNames ;

     for(auto elem : list){
         return_n_names.push_back(elem.toStdString());
         ui->n_lineEdit->insert((elem) + "...");
   }
}

void opening_window::on_choose_o_Button_clicked()
{
    QStringList list = QFileDialog::getOpenFileNames(0, "Open Dialog", "", "*.1?o");
    std::vector<std::string> oFileNames ;

     for(auto elem : list){
         return_o_names.push_back(elem.toStdString());
         ui->o_lineEdit->insert((elem) + "...");
   }
}

void opening_window::on_ok_Button_clicked()
{
    ui->ok_Button->setEnabled(false);
    emit this->operate(return_g_names, return_n_names, return_o_names);
    ui->stackedWidget->setCurrentIndex(1);
}

void opening_window::on_Check_Button_clicked(){
    QVector<QPair<QVector<double>, QVector<double>>> tecCRForPlot(24);
    QVector<QPair<QVector<double>, QVector<double>>> tecLRForPlot(24);
    QVector<QPair<QVector<double>, QVector<double>>> tecCGForPlot(32);
    QVector<QPair<QVector<double>, QVector<double>>> tecLGForPlot(32);

    for(auto &tecrval: tecr){
        auto moment = tecrval.moment.getUTCTime();
        for(int i=0; i<rCheckBoxes.size(); i++){
            if(rCheckBoxes[i]->isChecked()){
                if(!std::isnan(tecrval.sat[i].tecC)){
                    tecCRForPlot[i].first.append(moment);
                    tecCRForPlot[i].second.append(tecrval.sat[i].tecC);
                }
                if(!std::isnan(tecrval.sat[i].tecL)){
                    tecLRForPlot[i].first.append(moment);
                    tecLRForPlot[i].second.append(tecrval.sat[i].tecL);
                }
            }
        }
    }

    for(auto &tecgval: tecg){
        auto moment = tecgval.moment.getUTCTime();
        for(int i=0; i<gCheckBoxes.size(); i++){
            if(gCheckBoxes[i]->isChecked()){
                if(!std::isnan(tecgval.sat[i].tecC)){
                    tecCGForPlot[i].first.append(moment);
                    tecCGForPlot[i].second.append(tecgval.sat[i].tecC);
                }
                if(!std::isnan(tecgval.sat[i].tecL)){
                    tecLGForPlot[i].first.append(moment);
                    tecLGForPlot[i].second.append(tecgval.sat[i].tecL);
                }
            }
        }
    }

    auto startMoment = tecr[0].moment.getUTCTime();
    w.make_plot(startMoment, tecCRForPlot, tecLRForPlot, tecCGForPlot, tecLGForPlot);
    w.show();
}

void opening_window::handleResult(std::vector<TECvalR> TECR, std::vector<TECvalG> TECG)
{
    this->tecr = TECR;
    this->tecg = TECG;
    ui->Check_Button->setEnabled(true);
}

void opening_window::on_choose_i_Button_clicked()
{
    QStringList list = QFileDialog::getOpenFileNames(0, "Open Dialog", "", "*.1?i");
    std::vector<std::string> iFileNames ;

     for(auto elem : list){
         return_i_names.push_back(elem.toStdString());
         ui->i_lineEdit->insert((elem) + "...");
   }
}
