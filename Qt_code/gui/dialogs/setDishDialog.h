#ifndef SETDISHDIALOG_H
#define SETDISHDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "core/RestaurantSystem.h"

class setDishDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setDishDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~setDishDialog();

private slots:
    void showAddDish();   // 显示添加菜品界面
    void showRemoveDish(); // 显示删除菜品界面
    void showEditDish();  // 显示编辑菜品界面

    void addDish();    // 添加菜品
    void removeDish(); // 删除菜品
    void editDish();   // 编辑菜品

private:
    RestaurantSystem* system_;  // 后端系统接口

    // 控件声明
    QPushButton* addButton;     // 增加菜品按钮
    QPushButton* removeButton;  // 删除菜品按钮
    QPushButton* editButton;    // 编辑菜品按钮

    QWidget* mainWidget;        // 主界面显示区域
    QVBoxLayout* mainLayout;     // 主布局

    // 添加菜品的控件
    QLineEdit* nameLineEdit;    // 菜品名称输入框
    QLineEdit* priceLineEdit;   // 菜品价格输入框
    QPushButton* confirmAddButton;  // 确认添加按钮

    // 删除菜品的控件
    QListWidget* dishListWidget;   // 显示菜品的列表框
    QPushButton* confirmRemoveButton; // 确认删除按钮

    // 编辑菜品的控件
    QComboBox* dishSelectComboBox;   // 菜品选择框
    QLineEdit* editNameLineEdit;     // 编辑菜品名称输入框
    QLineEdit* editPriceLineEdit;    // 编辑菜品价格输入框
    QPushButton* confirmEditButton;  // 确认编辑按钮
};

#endif // SETDISHDIALOG_H
