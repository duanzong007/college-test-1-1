#include "adminpage.h"
#include "ui_adminpage.h"
#include "gui/dialogs/setTableDialog.h"
#include "gui/dialogs/setDishDialog.h"
#include "gui/dialogs/setDrinkDialog.h"
#include "gui/dialogs/addUserDialog.h"
#include "gui/dialogs/deleteUserDialog.h"
#include "gui/dialogs/passwordDialog.h"

adminpage::adminpage(RestaurantSystem *system, QWidget *parent)
    : QWidget(parent), ui(new Ui::adminpage), system_(system)
{
    ui->setupUi(this);


    connect(ui->setTable,  &QPushButton::clicked,this,&adminpage::Button_setTableDialog_clicked);
    connect(ui->setDish,   &QPushButton::clicked,this,&adminpage::Button_setDishDialog_clicked);
    connect(ui->setDrink,  &QPushButton::clicked,this,&adminpage::Button_setDrinkDialog_clicked);
    connect(ui->addUser,   &QPushButton::clicked,this,&adminpage::Button_addUserDialog_clicked);
    connect(ui->deleteUser,&QPushButton::clicked,this,&adminpage::Button_deleteUserDialog_clicked);
    connect(ui->password,  &QPushButton::clicked,this,&adminpage::Button_passwordDialog_clicked);
}

void adminpage::Button_setTableDialog_clicked()
{
    setTableDialog dlg(system_, this);
    dlg.exec();
}

void adminpage::Button_setDishDialog_clicked()
{
    setDishDialog dlg(system_, this);
    dlg.exec();
}

void adminpage::Button_setDrinkDialog_clicked()
{
    setDrinkDialog dlg(system_, this);
    dlg.exec();
}

void adminpage::Button_addUserDialog_clicked()
{
    addUserDialog dlg(system_, this);
    dlg.exec();
}

void adminpage::Button_deleteUserDialog_clicked()
{
    deleteUserDialog dlg(system_, this);
    dlg.exec();
}

void adminpage::Button_passwordDialog_clicked()
{
    passwordDialog dlg(system_, this);
    dlg.exec();
}

adminpage::~adminpage()
{
    delete ui;
}

