#ifndef OPENING_WINDOW_H
#define OPENING_WINDOW_H

#include <iostream>
#include <fstream>
#include <string>

#include <QApplication>
#include <QFileDialog >
#include <QMainWindow>
#include <QThread>
#include <QPointer>
#include <QCheckBox>

#include "mainwindow.h"
#include "worker.h"

Q_DECLARE_METATYPE(std::vector<std::string>)
Q_DECLARE_METATYPE(QVector<double>)
Q_DECLARE_METATYPE(std::vector<TECvalG>)
Q_DECLARE_METATYPE(std::vector<TECvalR>)

QT_BEGIN_NAMESPACE
namespace Ui { class opening_window; }
QT_END_NAMESPACE

class opening_window : public QMainWindow
{
    Q_OBJECT

public:
    opening_window(QWidget *parent = nullptr);
    ~opening_window();

    std::vector<std::string> return_g_names;
    std::vector<std::string> return_n_names;
    std::vector<std::string> return_o_names;
    std::vector<std::string> return_i_names;

    int number_of_satellite;
    QVector<double> tecC_forPlot, tecL_forPlot,time_forPlot;
private slots:
    void on_choose_G_Button_clicked();
    void on_choose_N_Button_clicked();
    void on_choose_o_Button_clicked();
    void on_ok_Button_clicked();
    void on_Check_Button_clicked();
    void handleResult(std::vector<TECvalR>, std::vector<TECvalG>);
    void on_choose_i_Button_clicked();

signals:
    void operate(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);
private:
    std::vector<TECvalR> tecr;
    std::vector<TECvalG> tecg;

    QVector<QPointer<QCheckBox>> rCheckBoxes;
    QVector<QPointer<QCheckBox>> gCheckBoxes;
    MainWindow w;
    Ui::opening_window *ui;
    QThread workerThread;
};

#endif // OPENING_WINDOW_H
