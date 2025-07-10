#ifndef CHECKORDERDIALOG_H
#define CHECKORDERDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

#include "core/RestaurantSystem.h"

class QComboBox;
class QTableWidget;
class QLineEdit;
class QPushButton;
class QLabel;

class checkOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit checkOrderDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~checkOrderDialog();

private slots:
    void onTableChanged(int index);
    void onSettle();
    void onOrderItemChanged(QTableWidgetItem* item);

private:
    void setupUI();
    void loadTables();                // 加载含有未结算订单的桌子
    void loadOrdersForTable(int tableId); // 加载指定桌子的订单到表格
    double calculateTotalAmount() const;  // 根据复选框计算有效订单总金额

    RestaurantSystem* system_;
    QComboBox* tableComboBox; // 左上角选择要结算的桌子
    QTableWidget* orderTableWidget; // 显示订单的表格（6列：类型、名称、数量、总金额、状态、计入结算）
    QLabel* totalLabel;       // 显示计算的总金额（含可能的餐位费）
    QLineEdit* amountEdit;    // 收银员输入顾客支付金额
    QPushButton* settleButton;
    QPushButton* cancelButton;
};

#endif // CHECKORDERDIALOG_H
