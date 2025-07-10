#ifndef CVIEWORDERDIALOG_H
#define CVIEWORDERDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class QGroupBox;
class QTableWidget;
class QPushButton;

class CviewOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CviewOrderDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~CviewOrderDialog();

private:
    void setupUI();
    void loadOrdersForTable(int tableId);

    RestaurantSystem* system_;
    int tableId_;  // 当前消费者所属桌子的编号

    QGroupBox* dishGroupBox;         // 菜品订单区域
    QTableWidget* dishTableWidget;   // 菜品订单表格

    QGroupBox* drinkGroupBox;        // 酒水订单区域
    QTableWidget* drinkTableWidget;  // 酒水订单表格

    QPushButton* closeButton;
};

#endif // CVIEWORDERDIALOG_H
