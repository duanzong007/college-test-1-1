#include "cashierpage.h"
#include "ui_cashierpage.h"
#include "gui/dialogs/viewOrderDialog.h"
#include "gui/dialogs/checkOrderDialog.h"
#include "gui/dialogs/dailyOrderSummaryDialog.h"

cashierpage::cashierpage(RestaurantSystem *system, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cashierpage)
    , system_(system)
{
    ui->setupUi(this);

    connect(ui->viewOrder,        &QPushButton::clicked,this,&cashierpage::Button_viewOrderDialog_clicked);
    connect(ui->checkOrder,       &QPushButton::clicked,this,&cashierpage::Button_checkOrderDialog_clicked);
    connect(ui->dailyOrderSummary,&QPushButton::clicked,this,&cashierpage::Button_dailyOrderSummaryDialog_clicked);

}

void cashierpage::Button_viewOrderDialog_clicked()
{
    viewOrderDialog dlg(system_, this);
    dlg.exec();
}

void cashierpage::Button_checkOrderDialog_clicked()
{
    checkOrderDialog dlg(system_, this);
    dlg.exec();
}

void cashierpage::Button_dailyOrderSummaryDialog_clicked()
{
    dailyOrderSummaryDialog dlg(system_, this);
    dlg.exec();
}

cashierpage::~cashierpage()
{
    delete ui;
}
