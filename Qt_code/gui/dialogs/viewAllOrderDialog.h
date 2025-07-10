#ifndef VIEWALLORDERDIALOG_H
#define VIEWALLORDERDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include "core/RestaurantSystem.h"

class viewAllOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit viewAllOrderDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~viewAllOrderDialog();

private slots:
    void onCloseButtonClicked();  // 关闭按钮点击槽

private:
    void setupUI();  // 设置UI
    void populateOrderTable();  // 填充订单表格

    RestaurantSystem *system_;
    QTableWidget *orderTable;  // 用于显示订单的表格
    QPushButton *closeButton;  // 关闭按钮
};

#endif // VIEWALLORDERDIALOG_H
