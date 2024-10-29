#include <QTimer>
#include <QTime>
#include <QLCDNumber>
#include <QString>
#include <QPalette>
#include <cmath>
#include "ui_Sinus.h"
#include <QKeyEvent>
#include <iostream>
using namespace std;


class Sinus : public QWidget 
{
    Q_OBJECT

    public:
      Sinus(QWidget *parent = 0);

    public slots:
    void modificar_amplitud();
    void modificar_fase();
    void actualitzar();
    void modificar_timer();
    void timers();
    void iniciar();
      

    private:
      float Amplitud = 0.5;
      int Fase = 0;
      QTimer *timer;
      int time = 0;
      bool active = false;
      Ui::Sinus ui;
   
};