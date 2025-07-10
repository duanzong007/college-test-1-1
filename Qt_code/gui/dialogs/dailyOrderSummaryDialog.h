#ifndef DAILYORDERSUMMARYDIALOG_H
#define DAILYORDERSUMMARYDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "core/RestaurantSystem.h"

class dailyOrderSummaryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit dailyOrderSummaryDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~dailyOrderSummaryDialog();

private slots:
    void onClose();

private:
    void setupUI();
    void generateSummary();

    RestaurantSystem *system_;
    QTableWidget *tableWidget;  // 用于显示订单记录的表格
    QLabel *totalLabel;         // 显示总金额
    QPushButton *closeButton;   // 关闭按钮
};

#endif // DAILYORDERSUMMARYDIALOG_H
