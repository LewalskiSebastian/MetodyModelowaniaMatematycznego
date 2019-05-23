#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include <math.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <fstream>
#include <QDateTime>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->pasekPostepu->hide();

    MainWindow::checkValidation();
    MainWindow::drawIcons();
    MainWindow::makePlot();

    timer = new QTimer(this);

    connect(ui->start, SIGNAL(clicked()), this, SLOT(makePlot()));
    //connect(ui->wizualizacja, SIGNAL(clicked()), this, ui->horizontalSlider()));
    connect(ui->wizualizacja, SIGNAL(clicked()), this, SLOT(timerWizualizacja()));
    connect(timer, SIGNAL(timeout()), this, SLOT(wizualizacja()));
    connect(ui->zapisz, SIGNAL(clicked()), this, SLOT(zapisz()));
    /*
     connect(ui->sine, SIGNAL(clicked()), this, SLOT(makePlot()));
     connect(ui->square, SIGNAL(clicked()), this, SLOT(makePlot()));
     connect(ui->step, SIGNAL(clicked()), this, SLOT(makePlot()));
     connect(ui->lineEdit_A1, SIGNAL(editingFinished()), this, SLOT(makePlot()));  // editingFinished lub returnPressed ?
     connect(ui->lineEdit_A2, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_time, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_samples, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_period, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     connect(ui->lineEdit_ampl, SIGNAL(editingFinished()), this, SLOT(makePlot()));
     */
    connect(ui->sine, SIGNAL(clicked()), this, SLOT(drawSpecialButtons()));
    connect(ui->square, SIGNAL(clicked()), this, SLOT(drawSpecialButtons()));
    connect(ui->step, SIGNAL(clicked()), this, SLOT(drawSpecialButtons()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerWizualizacja()
{
    double skala = (ui->lineEdit_skala->text()).toDouble();
    okresWizual = t*skala*1000/(ui->horizontalSlider->maximum()*1.0);
    wizual = 0;
    if (okresWizual > 10) timer->start(int(okresWizual));                   //dla okresu mniejszego niż 10ms QTimer i tak jest wywoływany co 10ms (wynika z sposobu działania QT)
    else timer->start(10);                                                  //Więc inaczej załatwimy większe "prędkości wizualizacji"
}

void MainWindow::wizualizacja()
{
    if (wizual < ui->horizontalSlider->maximum())
    {
        if (okresWizual > 10) wizual++;
        else wizual += 10/okresWizual;
        ui->horizontalSlider->setValue(wizual);
    }
    else timer->stop();
}

void MainWindow::zapisz()
{
    QDateTime data = QDateTime::currentDateTime();
    QString plik = QFileDialog::getOpenFileName(this, "Otwórz plik", "C://");
    QFile file(plik);

        if (file.open(QIODevice::ReadWrite | QIODevice::Append)) {
            QTextStream stream(&file);
            stream << data.toString("yyyy-MM-dd") << data.toString("hh:ss:mm") << endl;
            stream << "Metoda calkowania: ";
            if(ui->c_prostokat->isChecked())
                stream << "prostokatow" << endl;
            else if(ui->c_trapez->isChecked())
                stream << "trapezow" << endl;
            else if(ui->c_simpson->isChecked())
                stream << "Simpsona" << endl;
            else
                stream << "Runge-Kutta" << endl;
            stream << "Czestotliwosc probkowania: " << s << endl;
            stream << "Czas symulacji: " << t << endl;
            stream << "Aplituda: " << ampl << endl;
            stream << "Warunki pczatkowe: " << pocz << endl;
            stream << "Zwezka A1: " << A1 << endl;
            stream << "Zwezka A2: " << A2 << endl;
            stream << "Czas calkowan: " << ui->label_czas->text() << endl;
            stream << "Poziom wody pojemnika 1: " << endl;
            for(int i = 0; i <= t*s; i++){
                stream << h1[i] << endl;
            }
            stream << "Poziom wody pojemnika 2: " << endl;
            for(int i = 0; i <= t*s; i++){
                stream << h2[i] << endl;
            }
            QMessageBox::information(this, "Informacja","Zapis zakończony powodzeniem");
        } else QMessageBox::warning(this,"Błąd","Zapis zakończony niepowodzeniem");

  /*   fstream file;
     std::string current_locale_text = plik.toLocal8Bit().constData();
     file.open(current_locale_text,ios::out|ios::binary);
     file << "Line 1 goes here \n\n line 2 goes here";

     // or

     file << "Line 1";
     file << std::endl << std::endl;
     file << "Line 2";
     file.close();*/
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
    ui->lineEdit_skala->setValidator(hole);
    ui->lineEdit_pocz->setValidator(hole);
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
/*
double MainWindow::f(int x, double A, QVector<double> h, QVector<double> u1, QVector<double>& u2){      //funkcja co zwraca wartość funkcji z treści zadania
    //double g = 9.81;                                        //przyspieszenie ziemskie
    //double t = (ui->lineEdit_time->text()).toDouble();       // liczba sekund symulacji
    //double s = (ui->lineEdit_samples->text()).toDouble();    // liczba próbek na sekundę
    //double A1 = (ui->lineEdit_A1->text()).toDouble();      // pole przekroju odplywu nr1
    //double A2 = (ui->lineEdit_A2->text()).toDouble();      // pole przekroju odplywu nr2
    //double period = (ui->lineEdit_period->text()).toDouble(); // okres
    //double ampl = (ui->lineEdit_ampl->text().toDouble());       // amplituda pobudzenia
    double wynik = 0;
    if(h[x-1] >= pocz){                                   //Takie zabezpieczenie że jak mało jest wody w zbiorniku to żeby się nie dzieliło przez zero zamienia
        u2[x] = A*sqrt(2*g*h[x-1]);                         //poziom wody na wartość wpływającego strumienia
        wynik = ((u1[x]-A*sqrt(2*g*h[x-1]))/(3.14*h[x-1]*h[x-1]))*(1/s);
    }else{
        double h = pocz;
        u2[x] = A*sqrt(2*g*h);
        wynik = ((u1[x]-A*sqrt(2*g*h))/(3.14*h*h))*(1/s);
    }
    return wynik;
    //return 0;
} */

double MainWindow::f(int x, double A, double h, QVector<double> u1, QVector<double>& u2){
    double wynik = 0;
    if(h >= pocz){                                   //Takie zabezpieczenie że jak mało jest wody w zbiorniku to żeby się nie dzieliło przez zero zamienia
        u2[x] = A*sqrt(2*g*h);                         //poziom wody na wartość wpływającego strumienia
        wynik = ((u1[x]-A*sqrt(2*g*h))/(3.14*h*h))*(1/s);
    }else{
        double h = pocz;
        u2[x] = A*sqrt(2*g*h);
        wynik = ((u1[x]-A*sqrt(2*g*h))/(3.14*h*h))*(1/s);
    }
    return wynik;
}
/*
QVector<double> MainWindow::calkowanie(){               //to jest jeszcze nie gotowe, nie patrzeć!!!!!
    double g = 9.81;
    double s = 0;
    double st = 0;
    double samples = (ui->lineEdit_samples->text()).toDouble();
    double dx = 1/samples;
    int N = (ui->lineEdit_samples->text()).toInt()*(ui->lineEdit_time->text()).toInt();     //ilosc próbek w sumie
    double x = 0;
    double x2 = 0;
    double u = 1;           //wejscie
    double A1 = 0.01;       //zwężka 1
    QVector<double> h(N+1);
    QVector<double> h2(N+1);
    h[0] = 0.005;           //warunki początkowe
    h2[0] = 0.005;           //warunki początkowe
    for(int i = 1; i <= N; i++)
      {
        x = i* 1.0 * dx;
        x2 = (i * 1.0 - 0.5) * dx;
        //x = xp + i * dx;
        double przyrost2 = ((u-A1*sqrt(2*g*h2[i-1]))/(3.14*h2[i-1]*h2[i-1]))*(1/samples);
        st += przyrost2;
        h2[i] = st;
        //st += f(x - dx / 2);
        if(i < N){
            double przyrost = ((u-A1*sqrt(2*g*h[i-1]))/(3.14*h[i-1]*h[i-1]))*(1/samples);
            s += przyrost;
            h[i] = dx / 6 * (0 + ((u-A1*sqrt(2*g*h[i-1]))/(3.14*h[i-1]*h[i-1]))*(1/samples) + 2 * s + 4 * st);
        }
      }
      s = dx / 6 * (0 + ((u-A1*sqrt(2*g*h[N-1]))/(3.14*h[N-1]*h[N-1]))*(1/samples) + 2 * s + 4 * st);
      //s = dx / 6 * (f(xp) + f(xk) + 2 * s + 4 * st);
      return h;
}
*/
QVector<double> MainWindow::calkowanieKwadrat(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2){        //to jest całkowanie metodą kwadratów
    double calkaMax = 0;                                //wartość maks całki potrzebna do narysowania wykresu (maks podziałki pionowej)
    double uMax = 0;                                    //wartość maks całki potrzebna do narysowania wykresu (maks podziałki pionowej)
    //int t = (ui->lineEdit_time->text()).toInt();       // liczba sekund symulacji
    //int s = (ui->lineEdit_samples->text()).toInt();    // liczba próbek na sekundę
    double calka = 0;
    for(int i=1; i<t*s+1; i++){                         //tutaj całkujemy metodą kwadratów
        calka += f(i, A, h[i-1], u1, u2);
        h[i] = calka;
        if(calkaMax < calka) calkaMax = calka;
        if(uMax < u2[i]) uMax = u2[i];
        ui->pasekPostepu->setValue(100*i/(t*s));
        wartosci_koniec[0] = calka;
    }
    QVector<double> max(2);
    max[0] = calkaMax;
    max[1] = uMax;
    return max;
}

QVector<double> MainWindow::calkowanieTrapez(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2){
    double calkaMax = 0;
    double uMax = 0;
    double calka = 0;
    double calka2 = 0;
    double pole = 0;
    for(int i = 2; i < t*s+1; i++){
        calka = f(i, A, h[i-1], u1, u2)*s;
        calka2 = f(i-1, A, h[i-1], u1, u2)*s;
        pole += (calka + calka2)/(2*s);
        h[i] = pole;
        if(i == 2) h[1] = pole;
        if(calkaMax < pole) calkaMax = pole;
        if(uMax < u2[i]) uMax = u2[i];
        ui->pasekPostepu->setValue((100*i)/(t*s));
        wartosci_koniec[1] = pole;
    }
    QVector<double> max(2);
    max[0] = calkaMax;
    max[1] = uMax;
    return max;
}

QVector<double> MainWindow::calkowanieSimpson(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2){
    double calkaMax = 0;
    double uMax = 0;
    double calka = 0;
    double calka2 = 0;
    double calka3 = 0;
    double pole = 0;
    for(int i = 3; i < t*s+1; i++){

        calka = f(i, A, h[i-1], u1, u2)*s;
        calka2 = f(i-1, A, h[i-1], u1, u2)*s;
        calka3 = f(i-2, A, h[i-1], u1, u2)*s;
        pole += 1/(3*s*2)*(calka3 + 4*calka2 + calka);
        h[i] = pole;
        //h[i - 1] = pole;
        //
        if(i == 3){
            h[1] = pole;        // ?? jak w Runge Kutta
            h[2] = pole;
        }
        if(calkaMax < pole) calkaMax = pole;
        if(uMax < u2[i]) uMax = u2[i];
        ui->pasekPostepu->setValue((100*i)/(t*s)+1);
        wartosci_koniec[2] = pole;
    }
    QVector<double> max(2);
    max[0] = calkaMax;
    max[1] = uMax;
    return max;
}

QVector<double> MainWindow::calkowanieRungeKutha(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2){
    double calkaMax = 0;
    double uMax = 0;
    double K1 = 0;
    double K2 = 0;
    double K3 = 0;
    double K4 = 0;
    double pole = 0;
    for(int i = 3; i < t*s+1; i++){

        K1 = f(i-2, A, h[i-1], u1, u2);
        K2 = f(i-1, A, h[i-1] + 0.5*K1, u1, u2);
        K3 = f(i-1, A, h[i-1] + 0.5*K2, u1, u2);
        K4 = f(i, A, h[i-1] + K3, u1, u2);

        pole += 1.0/6.0*(K1 + 2*K2 + 2*K3 + K4);
        h[i] = pole;

        if(i == 3) {
            h[1] = pole;
            h[2] = pole;        // ?? zeby na początku nie wychodziły wartości jakieś dziwne xd ??
        }
        if(calkaMax < pole) calkaMax = pole;
        if(uMax < u2[i]) uMax = u2[i];
        ui->pasekPostepu->setValue((100*i)/(t*s)+1);
        wartosci_koniec[3] = pole;  // do wyswietlenia ostatniej wart w calce do porówania metod calkowania (aktualnie nie wystwietlam tego xd)
    }
    QVector<double> max(2);
    max[0] = calkaMax;
    max[1] = uMax;
    return max;
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
    int x = ui->horizontalSlider->value()*t*s/ui->horizontalSlider->maximum();
    int polozenie_x = pozycja.x();
    int polozenie_y = pozycja.y()+15;
    int wysokosc = 144;
    double poziom1 = 0;
    double poziom2 = 0;
    if (h1[x] >= pocz) poziom1 = wysokosc*(h1[x]/calka1max);
    if (h2[x] >= pocz) poziom2 = wysokosc*(h2[x]/calka2max);
    //int wypelnienie = ui->horizontalSlider->value();
    painter.drawRect(QRect(polozenie_x+48, polozenie_y+52+wysokosc-poziom1, 191, poziom1));
    painter.drawRect(QRect(polozenie_x+280, polozenie_y+229+wysokosc-poziom2, 191, poziom2));

    //double s = (ui->lineEdit_samples->text()).toDouble();    // liczba próbek na sekundę
    //double period = (ui->lineEdit_period->text()).toDouble(); // okres
    //double ampl = (ui->lineEdit_ampl->text().toDouble());       // amplituda pobudzenia
    /*
    double u = 0;
    if (ui->sine->isChecked() == TRUE)
    {
        u = 50*sin(x*2*3.1415/(period*s) )+50;
    }
    else if (ui->step->isChecked() == TRUE)
    {
        u = 100;
    }
    else if (ui->square->isChecked() == TRUE)
    {
        if (int(x)%(int(period*s)) <= int(period*s/2))   u = 100;
        else u = 0;
    }
    */
    int gruboscPen = (u1[x]*29/ampl)-1;
    painter.setPen(QPen(Qt::blue, gruboscPen));
    painter.drawLine(polozenie_x+144, polozenie_y+47, polozenie_x+144, polozenie_y+144+150);
    gruboscPen = ((u2[x]/u2max)*29)-1;
    painter.setPen(QPen(Qt::blue, gruboscPen));
    painter.drawLine(polozenie_x+375, polozenie_y+223, polozenie_x+375, polozenie_y+223+150);
    gruboscPen = ((u3[x]/u3max)*29)-1;
    painter.setPen(QPen(Qt::blue, gruboscPen));
    painter.drawLine(polozenie_x+507, polozenie_y+400, polozenie_x+507, polozenie_y+400+150);

}

void MainWindow::makePlot()
{
    ui->gotowe->setText("Całka pierwsza");

    drawSpecialButtons();
    checkValidation();

    /*
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
            u = ampl*sin(i*2*3.1415/(period*s) )+ampl;
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
          //else
          //przyrost = ((u-A1*sqrt(2*g*h1[i-1]))/(3.14*h1[i-1]*h1[i-1]))*(1/s);
          przyrost = f(i, A1, h1);
          calka = h1[i-1]+przyrost;
          h1[i] = calka;

          //if(h2[i-1]==0) przyrost2 = A1*sqrt(2*g*h1[i-1]);
          //else
          przyrost2 = ((A1*sqrt(2*g*h1[i-1])-A2*sqrt(2*g*h2[i-1]))/(3.14*h2[i-1]*h2[i-1]))*(1/s);
          calka2 = h2[i-1]+przyrost2;
          h2[i] = calka2;
          if(calka1max < calka) calka1max = calka;
          if(calka2max < calka2) calka2max = calka2;
          if(calka < 0.01) koniec = i/s;
        }
    */
    t = (ui->lineEdit_time->text()).toDouble();       // liczba sekund symulacji
    s = (ui->lineEdit_samples->text()).toDouble();    // liczba próbek na sekundę
    period = (ui->lineEdit_period->text()).toDouble(); // okres
    ampl = (ui->lineEdit_ampl->text().toDouble());       // amplituda pobudzenia
    A1 = (ui->lineEdit_A1->text()).toDouble();      // pole przekroju odplywu nr1
    A2 = (ui->lineEdit_A2->text()).toDouble();      // pole przekroju odplywu nr2
    pocz = (ui->lineEdit_pocz->text()).toDouble();     // warunki początkowe
    calka1max = 0.0;
    calka2max = 0.0;
    u2max = 0.0;
    u3max = 0.0;

 /*   u1.clear();
    u2.clear();
    u3.clear();
    h1.clear();
    h2.clear(); */
    u1.resize(t*s+1);                                                   //Wektor strumienia wejsciowego wody (pobudzenia)
    u2.resize(t*s+1);                                                   //Wektor strumienia wejściowego wody do drugiego pojemnika
    u3.resize(t*s+1);                                                   //Wektor strumienia wyjściowego wody z drugiego pojemnika (potrzebny tylko do wizualizacji)
    h1.resize(t*s+1);                                                   //Wektor wysokości wody w pierwszym pojemniku
    h2.resize(t*s+1);                                                   //Wektor wysokości wody w drugim pojemniku
    QVector<double> x(t*s+1);

    x[0] = 0.0;
    h1[0] = pocz;       //h(0) = y[0]  warunki początkowe
    h2[0] = pocz;
    u1[0] = 0.0;
    u2[0] = 0.0;
    u3[0] = 0.0;

    for(int i = 1; i<t*s+1; i++){                                               //Przypisujemy wartości wektorowi wejściowemu strumiania wody
        x[i] = double(i)/double(s);                 //tutaj to liczymy wartości jakie będą na poziomej podziałce
        if (ui->sine->isChecked() == TRUE)
        {
            u1[i] = (ampl/2)*sin(i*2*3.1415/(period*s) /*- 3.1415/2*/)+(ampl/2);
        }
        else if (ui->step->isChecked() == TRUE)
        {
            u1[i] = ampl;
        }
        else if (ui->square->isChecked() == TRUE)
        {
            if (int(i)%(int(period*s)) <= int(period*s/2))   u1[i] = ampl;
            else u1[i] = 0;
        }
    }

    QVector<double> max1(2);
    QVector<double> max2(2);

    QElapsedTimer czas;                                                             //timer służący do pomiaru czasu całkowania
    czas.start();
    //ui->pasekPostepu->show();
    if(ui->c_prostokat->isChecked())
        max1 = calkowanieKwadrat(A1, h1, u1, u2);                              //liczymy pierwszy wykres korzystając z metody kwadratów
    else if(ui->c_trapez->isChecked())
        max1 = calkowanieTrapez(A1, h1, u1, u2);
    else if(ui->c_simpson->isChecked())
        max1 = calkowanieSimpson(A1, h1, u1, u2);
    else
        max1 = calkowanieRungeKutha(A1, h1, u1, u2);
    //max1 = calkowanieKwadrat(A1, h1, u1, u2);                              //liczymy pierwszy wykres korzystając z metody kwadratów
    //max1 = calkowanieTrapez(A1, h1, u1, u2);
    //max1 = calkowanieSimpson(A1, h1, u1, u2);

    ui->gotowe->setText("Całka druga");
    //max2 = calkowanieKwadrat(A2, h2, u2, u3);                              //liczymy drugi wykres korzystając z metody kwadratów
    //max2 = calkowanieTrapez(A2, h2, u2, u3);
    //max2 = calkowanieSimpson(A2, h2, u2, u3);
    if(ui->c_prostokat->isChecked())
        max2 = calkowanieKwadrat(A2, h2, u2, u3);                              //liczymy drugi wykres korzystając z metody kwadratów
    else if(ui->c_trapez->isChecked())
        max2 = calkowanieTrapez(A2, h2, u2, u3);
    else if(ui->c_simpson->isChecked())
        max2 = calkowanieSimpson(A2, h2, u2, u3);
    else
        max2 = calkowanieRungeKutha(A2, h2, u2, u3);
    //ui->pasekPostepu->hide();
    ui->label_czas->setText(QStringLiteral("%1 ms").arg(czas.elapsed()));



    QWidget::update();

    calka1max = max1[0];
    calka2max = max2[0];
    u2max = max1[1];
    u3max = max2[1];

    // create graph and assign data to it:
    ui->customPlot->addGraph();                                             //Tutaj będziemy rysowali wykresy korzystając z customPlot
    ui->customPlot->graph(0)->setData(x, h1);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("czas [s]");
    ui->customPlot->yAxis->setLabel("wysokość słupa wody w zbiorniku 1 [m]");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, t);
    ui->customPlot->yAxis->setRange(0, calka1max);
    ui->customPlot->replot();

    ui->customPlot2->addGraph();
    ui->customPlot2->graph(0)->setData(x, h2);
    // give the axes some labels:
    ui->customPlot2->xAxis->setLabel("czas [s]");
    ui->customPlot2->yAxis->setLabel("wysokość słupa wody w zbiorniku 2 [m]");
    // set axes ranges, so we see all data:
    ui->customPlot2->xAxis->setRange(0, t);
    ui->customPlot2->yAxis->setRange(0, calka2max);
    ui->customPlot2->replot();
    ui->gotowe->setText("Gotowe!");
}



void MainWindow::on_horizontalSlider_valueChanged(int value)        //Jak się ruszy sliderem to wykonuje paintEvent od nowa
{
    QWidget::update();
}
/*
void Wektory::wpiszu1(QVector<double> u){
    u1 = u;
}

void Wektory::wpiszu2(QVector<double> u){
    u2 = u;
}

void Wektory::wpiszu3(QVector<double> u){
    u3 = u;
}

void Wektory::wpiszh1(QVector<double> h){
    h1 = h;
}

void Wektory::wpiszh2(QVector<double> h){
    h2 = h;
}

QVector<double> Wektory::podaju1(){
    return u1;
}

QVector<double> Wektory::podaju2(){
    return u2;
}

QVector<double> Wektory::podaju3(){
    return u3;
}

QVector<double> Wektory::podajh1(){
    return h1;
}

QVector<double> Wektory::podajh2(){
    return h2;
}
*/

