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
    double f(int x, double A, double h, QVector<double> u1, QVector<double>& u2);  // wcześniej f_runge
    QVector<double> u1;                                                   //Wektor strumienia wejsciowego wody (pobudzenia)
    QVector<double> u2;                                                   //Wektor strumienia wejściowego wody do drugiego pojemnika
    QVector<double> u3;                                                   //Wektor strumienia wyjściowego wody z drugiego pojemnika (potrzebny tylko do wizualizacji)
    QVector<double> h1;                                                   //Wektor wysokości wody w pierwszym pojemniku
    QVector<double> h2;                                                   //Wektor wysokości wody w drugim pojemniku
    double u2max = 0.0;                                    //maksywalna wartośc strumienia wypływającego potrzebna do wizualizacji
    double u3max = 0.0;
    double calka1max = 0.0;                                //maksywalna wartośc całki potrzebna do określenia wysokości wykresu
    double calka2max = 0.0;

    double t = 120;             //czas symulacji
    double s = 100;             //liczba próbek
    double period = 10;         //okres
    double A1 = 0.08;           //zwężka A1
    double A2 = 0.07;           //zwężka A2
    double pocz = 0.1;          //warunki początkowe
    double skala = 1;           //wsp. skali czasu
    double ampl = 1;            //wartość amplitudy
    double g = 9.81;            //przyspeiszenie ziemskie

    double okresWizual = 0;
    int wizual = 0;

private slots:
    void makePlot();
    void drawIcons();
    void checkValidation();
    void drawSpecialButtons();
    void timerWizualizacja();
    void wizualizacja();
    void zapisz();
    void zapiszCSV();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
};
#endif // MAINWINDOW_H
