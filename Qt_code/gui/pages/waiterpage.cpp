#include "waiterpage.h"
#include "ui_waiterpage.h"
#include "gui/dialogs/addOrderDialog.h"
#include "gui/dialogs/editDishStatusDialog.h"
#include "gui/dialogs/viewOrderDialog.h"

waiterpage::waiterpage(RestaurantSystem *system, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::waiterpage)
    , system_(system)
{
    ui->setupUi(this);


    connect(ui->addOrder,      &QPushButton::clicked,this,&waiterpage::Button_addOrderDialog_clicked);
    connect(ui->editDishStatus,&QPushButton::clicked,this,&waiterpage::Button_editDishStatusDialog_clicked);
    connect(ui->viewOrder,     &QPushButton::clicked,this,&waiterpage::Button_viewOrderDialog_clicked);
}

void waiterpage::Button_addOrderDialog_clicked()
{
    addOrderDialog dlg(system_, this);
    dlg.exec();
}

void waiterpage::Button_editDishStatusDialog_clicked()
{
    editDishStatusDialog dlg(system_, this);
    dlg.exec();
}

void waiterpage::Button_viewOrderDialog_clicked()
{
    viewOrderDialog dlg(system_, this);
    dlg.exec();
}

waiterpage::~waiterpage()
{
    delete ui;
}
