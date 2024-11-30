#ifndef COLORS_H
#define COLORS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Colors;
}
QT_END_NAMESPACE

class Colors : public QMainWindow
{
    Q_OBJECT

public:
    Colors(QWidget *parent = nullptr);
    ~Colors();

private:
    Ui::Colors *ui;
};
#endif // COLORS_H
