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


    MainWindow::checkValidation();
    MainWindow::drawIcons();
    MainWindow::makePlot();


     connect(ui->sine, SIGNAL(clicked()), this, SLOT(makePlot()));
     connect(ui->square, SIGNAL(clicked()), this, SLOT(makePlot()));
     connect(ui->step, SIGNAL(clicked()), this, SLOT(makePlot()));
     connect(ui->lineEdit_A1, SIGNAL(editingFinished()), this, SLOT(makePlot()));  // editingFinished lub returnPressed ?
     connect(ui->lineEdit_A2, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_time, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_samples, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_period, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_ampl, SIGNAL(editingFinished()), this, SLOT(makePlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawIcons()
{
    ui->sine->setIcon(QIcon(":/resources/pic/sine.png"));
    ui->sine->setIconSize(QSize(95, 150));
    ui->square->setIcon(QIcon(":/resources/pic/square.png"));
    ui->square->setIconSize(QSize(80, 150));
    ui->step->setIcon(QIcon(":/resources/pic/step.png"));
    ui->step->setIconSize(QSize(80, 150));
}


void MainWindow::checkValidation()
{
    QDoubleValidator *hole= new QDoubleValidator(this);             // do odpływów ze zbiorników
    QIntValidator *parameters= new QIntValidator(0, 1000, this);     // do liczby sekund symulacji i liczby próbek
    QDoubleValidator *parameters2 = new QDoubleValidator(this);      // do amplitudy i okresu
    QLocale curLocale(QLocale("en_GB"));
    QLocale::setDefault(curLocale);
    hole->setNotation(QDoubleValidator::StandardNotation);
    hole->setRange(0.0, 10.0, 4);
    parameters2->setNotation(QDoubleValidator::StandardNotation);
    parameters2->setRange(0.0, 100.0, 4);
    ui->lineEdit_A1->setValidator(hole);           // A1
    ui->lineEdit_A2->setValidator(hole);           // A2
    ui->lineEdit_time->setValidator(parameters);
    ui->lineEdit_samples->setValidator(parameters);
    ui->lineEdit_ampl->setValidator(parameters2);
    ui->lineEdit_period->setValidator(parameters2);
}

void MainWindow::drawSpecialButtons()           // dla sinusoidy i fali prostokątnej rysuje dodatkowe pole(a)
{
    if (ui->sine->isChecked() == TRUE)
    {
        ui->lineEdit_period->setVisible(TRUE);
        ui->label_7->setVisible(TRUE);
    }
    else if(ui->square->isChecked() == TRUE)
    {
        ui->lineEdit_period->setVisible(TRUE);
        ui->label_7->setVisible(TRUE);
    }
    else if(ui->sine->isChecked() == FALSE)
    {
        ui->lineEdit_period->setVisible(FALSE);
        ui->label_7->setVisible(FALSE);
    }
    else if(ui->square->isChecked() == FALSE)
    {
        ui->lineEdit_period->setVisible(FALSE);
        ui->label_7->setVisible(FALSE);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)                 //Rysowanie prostokątów i linii w wizualizacji
{
    QPainter painter(this);

    QPoint pozycja = ui->picLabel->pos();                       //bierze pozycję picLabel z obrazkiem

    /*QPen pen;
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    QBrush brush;
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    */
    painter.setPen(QPen(Qt::blue, 0));
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    //int polozenie_x = 1000;
    //int polozenie_y = 182;
    int polozenie_x = pozycja.x();
    int polozenie_y = pozycja.y()+12;
    int wysokosc = 144;
    int wypelnienie = ui->horizontalSlider->value();
    painter.drawRect(QRect(polozenie_x+48, polozenie_y+52+wysokosc-wysokosc*wypelnienie/100, 191, wysokosc*wypelnienie/100));
    painter.drawRect(QRect(polozenie_x+280, polozenie_y+229+wysokosc-wysokosc*wypelnienie/100, 191, wysokosc*wypelnienie/100));
    int gruboscPen = (wypelnienie*29/100)-1;
    painter.setPen(QPen(Qt::blue, gruboscPen));
    painter.drawLine(polozenie_x+144, polozenie_y+47, polozenie_x+144, polozenie_y+144+150);
    painter.drawLine(polozenie_x+375, polozenie_y+223, polozenie_x+375, polozenie_y+223+150);
    painter.drawLine(polozenie_x+507, polozenie_y+400, polozenie_x+507, polozenie_y+400+150);

}

void MainWindow::makePlot()
{
    drawSpecialButtons();
    checkValidation();
    double t = (ui->lineEdit_time->text()).toDouble();       // liczba sekund symulacji
    double s = (ui->lineEdit_samples->text()).toDouble();    // liczba próbek na sekundę

    QVector<double> x(t*s+1), h1(t*s+1), h2(t*s+1);         // wektor przechowujacy wartosci danych do wykresu

    double A1 = (ui->lineEdit_A1->text()).toDouble();      // pole przekroju odplywu nr1
    double A2 = (ui->lineEdit_A2->text()).toDouble();      // pole przekroju odplywu nr2
    double period = (ui->lineEdit_period->text()).toDouble(); // okres
    double ampl = (ui->lineEdit_ampl->text().toDouble());       // amplituda pobudzenia

    double g = 9.81;
    double calka = 0.0;
    double przyrost = 0.0;
    double calka2 = 0.0;
    double przyrost2 = 0.0;
    double u;
    double calka1max = 0.0;
    double calka2max = 0.0;
    double koniec = t;
    x[0] = 0.0;
    h1[0] = 0.1;       //h(0) = y[0]  warunki początkowe
    h2[0] = 0.1;
    int okr = period*s;
    for (int i=1; i<t*s+1; i++)
        {
        if (ui->sine->isChecked() == TRUE)
        {
            u = ampl*sin(i*2*3.1415/(period*s) /*- 3.1415/2*/)+ampl;
        }
        else if (ui->step->isChecked() == TRUE)
        {
            u = ampl;
        }
        else if (ui->square->isChecked() == TRUE)
        {
            if (okr >= 0)   u = ampl;
            else if(okr >= -1000 && okr < 0) u = 0;
            else okr = period*s;
            okr--;
        }
        else u = 0;
          x[i] = i/s; // x goes from

         // if(h1[i-1]==0) przyrost = u;
          /*else*/ przyrost = ((u-A1*sqrt(2*g*h1[i-1]))/(3.14*h1[i-1]*h1[i-1]))*(1/s);
          calka = h1[i-1]+przyrost;
          h1[i] = calka;

          //if(h2[i-1]==0) przyrost2 = A1*sqrt(2*g*h1[i-1]);
          /*else*/ przyrost2 = ((A1*sqrt(2*g*h1[i-1])-A2*sqrt(2*g*h2[i-1]))/(3.14*h2[i-1]*h2[i-1]))*(1/s);
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
    ui->customPlot->xAxis->setLabel("czas [s]");
    ui->customPlot->yAxis->setLabel("wysokość słupa wody w zbiorniku 1 [m]");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, koniec);
    ui->customPlot->yAxis->setRange(0, calka1max);
    ui->customPlot->replot();

    ui->customPlot2->addGraph();
    ui->customPlot2->graph(0)->setData(x, h2);
    // give the axes some labels:
    ui->customPlot2->xAxis->setLabel("czas [s]");
    ui->customPlot2->yAxis->setLabel("wysokość słupa wody w zbiorniku 2 [m]");
    // set axes ranges, so we see all data:
    ui->customPlot2->xAxis->setRange(0, koniec);
    ui->customPlot2->yAxis->setRange(0, calka2max);
    ui->customPlot2->replot();
}


void MainWindow::on_horizontalSlider_valueChanged(int value)        //Jak się ruszy sliderem to wykonuje paintEvent od nowa
{
    QWidget::update();
}
