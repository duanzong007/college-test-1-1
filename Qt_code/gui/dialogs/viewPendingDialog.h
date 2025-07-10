#ifndef VIEWPENDINGDIALOG_H
#define VIEWPENDINGDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class QTableWidget;
class QPushButton;

class viewPendingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit viewPendingDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~viewPendingDialog();

private slots:
    void onRefresh();
    void onClose();

private:
    void setupUI();
    void loadPendingOrders();    // 加载状态为 "待做" 的订单（按下单时间升序）
    void updateCheckedOrders();  // 将复选框勾选的订单状态更新为 "已做" 并保存

    RestaurantSystem* system_;
    QTableWidget* pendingTableWidget;
    QPushButton* refreshButton;
    QPushButton* closeButton;
};

#endif // VIEWPENDINGDIALOG_H
