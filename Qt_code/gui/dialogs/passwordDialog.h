#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>

#include "core/RestaurantSystem.h"

class passwordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit passwordDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~passwordDialog();

private slots:
    void saveChanges();        // 保存更改

private:
    RestaurantSystem* system_;  // 后端系统接口

    // 控件声明
    QComboBox* userSelectComboBox;  // 用户选择框
    QLineEdit* newPasswordLineEdit;  // 新密码输入框
    QPushButton* saveButton;    // 保存按钮
};

#endif // PASSWORDDIALOG_H
