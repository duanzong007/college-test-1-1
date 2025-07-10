#include "consumerpage.h"
#include "ui_consumerpage.h"
#include "gui/dialogs/getTableDialog.h"
#include "gui/dialogs/CaddOrderDialog.h"
#include "gui/dialogs/CviewOrderDialog.h"

consumerpage::consumerpage(RestaurantSystem *system, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::consumerpage)
    , system_(system)
{
    ui->setupUi(this);

    connect(ui->getTable, &QPushButton::clicked,this,&consumerpage::Button_getTableDialog_clicked);
    connect(ui->CaddOrder,&QPushButton::clicked,this,&consumerpage::Button_CaddOrderDialog_clicked);
    connect(ui->viewOrder,&QPushButton::clicked,this,&consumerpage::Button_CviewOrderDialog_clicked);

}

void consumerpage::Button_getTableDialog_clicked()
{
    getTableDialog dlg(system_, this);
    dlg.exec();
}

void consumerpage::Button_CaddOrderDialog_clicked()
{
    CaddOrderDialog dlg(system_, this);
    dlg.exec();
}

void consumerpage::Button_CviewOrderDialog_clicked()
{
    CviewOrderDialog dlg(system_, this);
    dlg.exec();
}

consumerpage::~consumerpage()
{
    delete ui;
}
