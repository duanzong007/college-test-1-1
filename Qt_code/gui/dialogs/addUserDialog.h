#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

#include "core/RestaurantSystem.h"

class addUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addUserDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~addUserDialog();

private slots:
    void addUser();    // 添加用户的槽函数

private:
    RestaurantSystem* system_;  // 后端系统接口

    // 控件声明
    QLineEdit* usernameLineEdit;   // 用户名输入框
    QLineEdit* passwordLineEdit;   // 密码输入框
    QComboBox* roleComboBox;       // 角色选择框
    QPushButton* confirmButton;    // 确认按钮
};

#endif // ADDUSERDIALOG_H
