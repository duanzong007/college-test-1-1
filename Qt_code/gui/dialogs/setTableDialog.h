#ifndef SETTABLEDIALOG_H
#define SETTABLEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "core/RestaurantSystem.h"

class setTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setTableDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~setTableDialog();

private slots:
    void showAddTable();    // 显示添加餐位界面
    void showRemoveTable(); // 显示删除餐位界面
    void showEditTable();   // 显示编辑餐位界面

    void addTable();    // 添加餐位
    void removeTable(); // 删除餐位
    void editTable();   // 编辑餐位

private:
    RestaurantSystem* system_;  // 后端系统接口

    // 控件声明
    QPushButton* addButton;     // 增加餐位按钮
    QPushButton* removeButton;  // 删除餐位按钮
    QPushButton* editButton;    // 编辑餐位按钮

    QWidget* mainWidget;         // 主界面显示区域
    QVBoxLayout* mainLayout;     // 主布局

    // 添加餐位的控件
    QLineEdit* tableCountLineEdit;     // 添加的餐位数量
    QPushButton* confirmAddButton;     // 确认添加按钮

    // 删除餐位的控件
    QListWidget* tableListWidget;      // 显示餐位的列表框
    QPushButton* confirmRemoveButton;  // 确认删除按钮

    // 编辑餐位的控件
    QComboBox* tableSelectComboBox;     // 餐位选择框
    QLineEdit* editUserIdLineEdit;      // 编辑使用者ID输入框
    QComboBox* editStatusComboBox;      // 编辑餐位状态（空闲/占用/已预定）
    QPushButton* confirmEditButton;     // 确认编辑按钮
};

#endif // SETTABLEDIALOG_H
