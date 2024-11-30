#include "colors_codi.h"

colors_codi::colors_codi(QWidget *parent): QPushButton(parent) {}


QString colors_codi:: seguent_text(QString txt){
    if (txt == "Vermell") return "Groc";
    else if (txt == "Groc") return "Verd";
    else if (txt == "Verd") return "Cian";
    else if (txt == "Cian") return "Blau";
    else if (txt == "Blau") return "Magenta";
    else if (txt == "Magenta") return "Vermell";
    return "colorssssss";
}

QString colors_codi:: ant_txt(QString txt){
    if (txt == "Vermell") return "Magenta";
    else if (txt == "Magenta") return "Blau";
    else if (txt == "Blau") return "Cian";
    else if (txt == "Cian") return "Verd";
    else if (txt == "Verd") return "Groc";
    else if (txt == "Groc") return "Vermell";
    return "ALGO";
}

void colors_codi:: dial_modifica1(int val){
    int r = val - valor_ant;
    if (r!=6 && r!=-6 && r!=0){
        QString txt = text();
        QString nxttxt;
        if (r < 0) nxttxt = seguent_text(txt);
        else if (r > 0)nxttxt = ant_txt(txt);
        QColor color = colorMap[nxttxt];
        color = color.convertTo(QColor::Hsv);
        color.setHsv(color.hue(), color.saturation(), brightness);
        setStyleSheet(QString("background-color: %1;").arg(color.name()));
        setText(nxttxt);
        if (val == 6 && valor_ant == 5) valor_ant = 0;
        else if (val == 0 && valor_ant == 1) valor_ant = 6;
        else valor_ant = val;
    }
}


void colors_codi:: canviar_brightness(int b){
    brightness = b;
    QString txt = text();
    QColor color = colorMap[txt];
    color = color.convertTo(QColor::Hsv);
    color.setHsv(color.hue(), color.saturation(), brightness);
    setStyleSheet(QString("background-color: %1;").arg(color.name()));

}

