#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::makePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makePlot()
{
    double t = 120; //liczba sekund symulacji
    double s = 100; //liczba próbek na sekundę
    // generate some data:
    QVector<double> x(t*s+1), h1(t*s+1), h2(t*s+1); // initialize with entries 0..100
    srand (time(NULL));
    /*for (int i=0; i<101; ++i)
    {
      x[i] = i/10.0; // x goes from -1 to 1
      y[i] = i/100.0+rand()%100/1000.0+0.1; // let's plot a quadratic function
    }*/
    double A1 = 0.01;
    double A2 = 0.015;
    double g = 9.81;
    double b = 1.0;      //pole przekroju
    double calka = 0.0;
    double przyrost = 0.0;
    double calka2 = 0.0;
    double przyrost2 = 0.0;
    double u = 0.0;
    double calka1max = 0.0;
    double calka2max = 0.0;
    double koniec = t;
    x[0] = 0.0;
    h1[0] = 0.1;       //h(0) = y[0]
    h2[0] = 0.1;
    for (int i=1; i<t*s+1; ++i)
        {
        if(i < 500){
            u = 1;
        }else{
            u = 0;
        }
          x[i] = i/s; // x goes from -1 to 1
          przyrost = ((u-A1*sqrt(2*g*h1[i-1]))/(3.14*h1[i-1]*h1[i-1]))*0.01;
          calka = h1[i-1]+przyrost;
          h1[i] = calka; // let's plot a quadratic function
          przyrost2 = ((A1*sqrt(2*g*h1[i-1])-A2*sqrt(2*g*h2[i-1]))/(3.14*h2[i-1]*h2[i-1]))*0.01;
          calka2 = h2[i-1]+przyrost2;
          h2[i] = calka2;
          if(calka1max < calka) calka1max = calka;
          if(calka2max < calka2) calka2max = calka2;
          if(calka < 0.01) koniec = i/s;
        }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, h1);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("czas");
    ui->customPlot->yAxis->setLabel("ilość wody w zbiorniku 1");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, koniec);
    ui->customPlot->yAxis->setRange(0, calka1max);
    ui->customPlot->replot();

    ui->customPlot2->addGraph();
    ui->customPlot2->graph(0)->setData(x, h2);
    // give the axes some labels:
    ui->customPlot2->xAxis->setLabel("czas");
    ui->customPlot2->yAxis->setLabel("ilość wody w zbiorniku 2");
    // set axes ranges, so we see all data:
    ui->customPlot2->xAxis->setRange(0, koniec);
    ui->customPlot2->yAxis->setRange(0, calka2max);
    ui->customPlot2->replot();
}
