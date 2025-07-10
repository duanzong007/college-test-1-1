#ifndef ADDORDERDIALOG_H
#define ADDORDERDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class QComboBox;
class QGroupBox;
class QSpinBox;
class QPushButton;

class addOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit addOrderDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~addOrderDialog();

private slots:
    void onSubmit();

private:
    void setupUI();

    RestaurantSystem* system_;
    QComboBox* tableComboBox;        // 选择桌号的下拉框
    QGroupBox* dishGroupBox;         // 菜品组
    QGroupBox* drinkGroupBox;        // 酒水组
    QPushButton* submitButton;       // 下单按钮
    QPushButton* cancelButton;       // 取消按钮

    // 用于记录每个菜品/酒水对应的点餐数量（key为菜品ID）
    QMap<int, QSpinBox*> dishSpinBoxes;
    QMap<int, QSpinBox*> drinkSpinBoxes;
};

#endif // ADDORDERDIALOG_H
