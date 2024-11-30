#ifndef COLORS_CODI_H
#define COLORS_CODI_H

#include <QPushButton>
#include <QString>
#include <QMap>


class colors_codi: public QPushButton
{
    Q_OBJECT
public:
    colors_codi(QWidget *parent = nullptr);

public slots:
    void dial_modifica1(int val);
    void canviar_brightness(int b);

private:
    int previous_value=1;
    QString ordre_colors[6] = {"Vermell", "Groc", "Verd", "Cian", "Blau", "Magenta"};
    QMap<QString, QColor> colorMap = {
        {"Vermell", QColor(255, 0, 0)},
        {"Groc", QColor(255, 255, 0)},
        {"Verd", QColor(0, 255, 0)},
        {"Cian", QColor(0, 255, 255)},
        {"Blau", QColor(0, 0, 255)},
        {"Magenta", QColor(255, 0, 255)}
    };    
    QString seguent_text(QString txt);
    QString ant_txt(QString txt);
    int brightness = 255;
    int valor_ant = 3;
};

#endif // COLORS_CODI_H
