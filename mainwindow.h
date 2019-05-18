#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPainter>
#include <QTimer>


namespace Ui {
class MainWindow;
//class Wektory;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    QVector<double> calkowanie();
    QVector<double> calkowanieKwadrat(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2);
    QVector<double> calkowanieTrapez(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2);
    QVector<double> calkowanieSimpson(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2);
    QVector<double> calkowanieRungeKutha(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2);
   // double f(int x, double A, QVector<double> h, QVector<double> u1, QVector<double>& u2);
    double f(int x, double A, double h, QVector<double> u1, QVector<double>& u2);  // wcześniej f_runge
    QVector<double> u1;                                                   //Wektor strumienia wejsciowego wody (pobudzenia)
    QVector<double> u2;                                                   //Wektor strumienia wejściowego wody do drugiego pojemnika
    QVector<double> u3;                                                   //Wektor strumienia wyjściowego wody z drugiego pojemnika (potrzebny tylko do wizualizacji)
    QVector<double> h1;                                                   //Wektor wysokości wody w pierwszym pojemniku
    QVector<double> h2;                                                   //Wektor wysokości wody w drugim pojemniku
    double u2max = 0.0;
    double u3max = 0.0;
    double calka1max = 0.0;                                //maksywalna wartośc całki potrzebna do określenia wysokości wykresu
    double calka2max = 0.0;

    double t = 120;
    double s = 100;
    double period = 10;
    double A1 = 0.08;
    double A2 = 0.07;
    double pocz = 0.1;
    double skala = 1;
    double ampl = 1;
    double g = 9.81;

    double okresWizual = 0;
    int wizual = 0;
    double wartosci_koniec[4]; // pamietaj aby zmienic !!!  lub usunąć ? xd

private slots:
    void makePlot();
    void drawIcons();
    void checkValidation();
    void drawSpecialButtons();
    void timerWizualizacja();
    void wizualizacja();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
};
/*
class Wektory
{
    Q_OBJECT

public:
    void wpiszu1(QVector<double> u);
    void wpiszu2(QVector<double> u);
    void wpiszu3(QVector<double> u);
    void wpiszh1(QVector<double> h);
    void wpiszh2(QVector<double> h);

    QVector<double> podaju1();
    QVector<double> podaju2();
    QVector<double> podaju3();
    QVector<double> podajh1();
    QVector<double> podajh2();

private slots:
    QVector<double> u1;                                                   //Wektor strumienia wejsciowego wody (pobudzenia)
    QVector<double> u2;                                                   //Wektor strumienia wejściowego wody do drugiego pojemnika
    QVector<double> u3;                                                   //Wektor strumienia wyjściowego wody z drugiego pojemnika (potrzebny tylko do wizualizacji)
    QVector<double> h1;                                                   //Wektor wysokości wody w pierwszym pojemniku
    QVector<double> h2;                                                   //Wektor wysokości wody w drugim pojemniku

private:
    Ui::Wektory *ui;
};
*/
#endif // MAINWINDOW_H
