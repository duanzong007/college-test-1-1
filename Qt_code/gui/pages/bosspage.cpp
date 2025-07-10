#include "bosspage.h"
#include "ui_bosspage.h"
#include "gui/dialogs/viewallinfodialog.h"
#include "gui/dialogs/viewallOrderdialog.h"
#include "gui/dialogs/dishRankingDialog.h"
#include "gui/dialogs/timeOrderDialog.h"

bosspage::bosspage(RestaurantSystem *system, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::bosspage)
    , system_(system)
{
    ui->setupUi(this);



    connect(ui->viewAllInfo, &QPushButton::clicked,this,&bosspage::Button_viewAllInfo_clicked);
    connect(ui->viewAllOrder,&QPushButton::clicked,this,&bosspage::Button_viewAllOrder_clicked);
    connect(ui->dishRanking, &QPushButton::clicked,this,&bosspage::Button_dishRankingDialog_clicked);
    connect(ui->timeOrder,   &QPushButton::clicked,this,&bosspage::Button_timeOrderDialog_clicked);
}

void bosspage::Button_viewAllInfo_clicked()
{
    viewAllInfoDialog dlg(system_, this);
    dlg.exec();
}

void bosspage::Button_viewAllOrder_clicked()
{
    viewAllOrderDialog dlg(system_, this);
    dlg.exec();
}
//dishRankingDialog
void bosspage::Button_dishRankingDialog_clicked()
{
    dishRankingDialog dlg(system_, this);
    dlg.exec();
}
//timeOrderDialog
void bosspage::Button_timeOrderDialog_clicked()
{
    timeOrderDialog dlg(system_, this);
    dlg.exec();
}



bosspage::~bosspage()
{
    delete ui;
}
