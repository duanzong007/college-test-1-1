#ifndef DELETEUSERDIALOG_H
#define DELETEUSERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>

#include "core/RestaurantSystem.h"

class deleteUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit deleteUserDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~deleteUserDialog();

private slots:
    void deleteUser();  // 删除用户的槽函数

private:
    RestaurantSystem* system_;  // 后端系统接口

    // 控件声明
    QListWidget* userListWidget;  // 用户列表框
    QPushButton* confirmDeleteButton;  // 确认删除按钮
};

#endif // DELETEUSERDIALOG_H
