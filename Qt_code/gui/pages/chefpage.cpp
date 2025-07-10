#include "chefpage.h"
#include "ui_chefpage.h"
#include "gui/dialogs/viewPendingDialog.h"
#include "gui/dialogs/dailyDishSummaryDialog.h"

chefpage::chefpage(RestaurantSystem *system, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chefpage)
    , system_(system)
{
    ui->setupUi(this);

    connect(ui->viewPending,     &QPushButton::clicked,this,&chefpage::Button_viewPendingDialog_clicked);
    connect(ui->dailyDishSummary,&QPushButton::clicked,this,&chefpage::Button_dailyDishSummaryDialog_clicked);

}

void chefpage::Button_viewPendingDialog_clicked()
{
    viewPendingDialog dlg(system_, this);
    dlg.exec();
}

void chefpage::Button_dailyDishSummaryDialog_clicked()
{
    dailyDishSummaryDialog dlg(system_, this);
    dlg.exec();
}

chefpage::~chefpage()
{
    delete ui;
}
