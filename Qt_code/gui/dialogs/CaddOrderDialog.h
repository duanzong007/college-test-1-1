#ifndef CADDORDERDIALOG_H
#define CADDORDERDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class QGroupBox;
class QSpinBox;
class QPushButton;
class QScrollArea;

class CaddOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CaddOrderDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~CaddOrderDialog();

private slots:
    void onSubmit();

private:
    void setupUI();

    RestaurantSystem* system_;
    QGroupBox* dishGroupBox;         // 菜品分组容器
    QGroupBox* drinkGroupBox;        // 酒水分组容器
    QPushButton* submitButton;       // 下单按钮
    QPushButton* cancelButton;       // 取消按钮

    // 用于记录每个菜品/酒水对应的点餐数量（key为菜品/酒水的id）
    QMap<int, QSpinBox*> dishSpinBoxes;
    QMap<int, QSpinBox*> drinkSpinBoxes;
};

#endif // CADDORDERDIALOG_H
