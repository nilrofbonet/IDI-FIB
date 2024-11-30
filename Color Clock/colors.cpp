#include "colors.h"
#include "ui_colors.h"

Colors::Colors(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Colors)
{
    ui->setupUi(this);
}

Colors::~Colors()
{
    delete ui;
}
