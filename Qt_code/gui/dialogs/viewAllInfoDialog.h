#ifndef VIEWALLINFODIALOG_H
#define VIEWALLINFODIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>

#include "core/RestaurantSystem.h"

class viewAllInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit viewAllInfoDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~viewAllInfoDialog();

private slots:
    void onUsersButtonClicked();
    void onTablesButtonClicked();
    void onDishesButtonClicked();
    void onDrinksButtonClicked();  // 新增槽函数用于切换酒水信息页面
    void onCloseButtonClicked();

private:
    void setupUI();
    void populateUsersTable();
    void populateTablesTable();
    void populateDishesTable();
    void populateDrinksTable();  // 新增填充酒水信息表格的函数

    RestaurantSystem *system_;
    QTableWidget *usersTable;
    QTableWidget *tablesTable;
    QTableWidget *dishesTable;
    QTableWidget *drinksTable;  // 新增酒水信息表格
    QStackedWidget *stackedWidget;

    QPushButton *usersButton;
    QPushButton *tablesButton;
    QPushButton *dishesButton;
    QPushButton *drinksButton;  // 新增酒水按钮
    QPushButton *closeButton;
};

#endif // VIEWALLINFODIALOG_H
