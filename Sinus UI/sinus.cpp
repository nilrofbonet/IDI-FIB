#include "sinus.h"

Sinus::Sinus(QWidget* parent): QWidget(parent) 
{
    ui.setupUi(this);
    actualitzar();

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &Sinus::timers);

}

void Sinus::modificar_amplitud(){
    Amplitud = (ui.ampli -> value())/10.0;
    actualitzar();
}

void Sinus::modificar_fase(){
    Fase = ui.faseh -> value();
    actualitzar();
}

void Sinus::actualitzar(){
    int val;
    val = sin(0+time+Fase)*100*Amplitud;
    ui.p1 ->setValue(val);
    
    val = sin(1+time+Fase)*100*Amplitud;
    ui.p2 ->setValue(val+time);

    val = sin(2+time+Fase)*100*Amplitud;
    ui.p3 ->setValue(val);

    val = sin(3+time+Fase)*100*Amplitud;
    ui.p4 ->setValue(val);

    val = sin(4+time+Fase)*100*Amplitud;
    ui.p5 ->setValue(val);

    val = sin(5+time+Fase)*100*Amplitud;
    ui.p6 ->setValue(val);

    val = sin(6+time+Fase)*100*Amplitud;
    ui.p7 ->setValue(val);

    val = sin(7+time+Fase)*100*Amplitud;
    ui.p8 ->setValue(val);

    val = sin(8+time+Fase)*100*Amplitud;
    ui.p9 ->setValue(val);

    val = sin(9+time+Fase)*100*Amplitud;
    ui.p10 ->setValue(val);
}

void Sinus::modificar_timer(){
  if(not active){
    timer->start();
    active = true;
  }
  else{
    timer->stop();
    active = false;
  };
    
}

void Sinus::timers(){
    time += 1;
    actualitzar();
}

void Sinus::iniciar(){
  Amplitud = 0.5;
  ui.ampli->setValue(Amplitud*10);
  Fase = 0;
  ui.faseh->setValue(Fase);
  time = 0;
  if (timer->isActive()) timer->stop();
  actualitzar();
}