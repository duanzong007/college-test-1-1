#ifndef VIEWORDERDIALOG_H
#define VIEWORDERDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class QComboBox;
class QGroupBox;
class QTableWidget;
class QPushButton;

class viewOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit viewOrderDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~viewOrderDialog();

private slots:
    void onTableSelectionChanged(int index);

private:
    void setupUI();
    void loadTableComboBox();         // 填充下拉框，仅显示"使用中"且有订单的桌子
    void loadOrdersForTable(int tableId); // 根据选择的桌子加载订单（分成菜品和酒水两个列表）

    RestaurantSystem* system_;
    QComboBox* tableComboBox;        // 桌号选择下拉框
    QGroupBox* dishGroupBox;         // 显示菜品订单的区域
    QTableWidget* dishTableWidget;   // 菜品订单表格
    QGroupBox* drinkGroupBox;        // 显示酒水订单的区域
    QTableWidget* drinkTableWidget;  // 酒水订单表格
    QPushButton* closeButton;
};

#endif // VIEWORDERDIALOG_H
