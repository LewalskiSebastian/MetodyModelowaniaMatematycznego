#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    QVector<double> calkowanie();
    double calkowanieKwadrat(double A, QVector<double>& h, QVector<double> u1, QVector<double>& u2);
    double f(int x, double A, QVector<double> h, QVector<double> u1, QVector<double>& u2);

private slots:
    void makePlot();
    void drawIcons();
    void checkValidation();
    void drawSpecialButtons();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
