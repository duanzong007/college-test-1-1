#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include "deleteUserDialog.h"

deleteUserDialog::deleteUserDialog(RestaurantSystem *system, QWidget *parent) :
    QDialog(parent), system_(system) {

    setWindowTitle("删除用户");

    // 创建布局和控件
    QVBoxLayout* layout = new QVBoxLayout(this);

    userListWidget = new QListWidget(this);  // 列出所有用户
    confirmDeleteButton = new QPushButton("确认删除", this);

    // 标签字体设置
    QLabel* label = new QLabel("选择要删除的用户：", this);
    label->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    // 设置控件字体
    userListWidget->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmDeleteButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label);
    layout->addWidget(userListWidget);
    layout->addWidget(confirmDeleteButton);

    // 获取所有用户并显示
    for (const User& user : system_->getUserManager().getUsers()) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(user.getUsername()));
        item->setCheckState(Qt::Unchecked);  // 默认不勾选
        userListWidget->addItem(item);
    }

    // 设置 `QListWidget` 为多选模式
    userListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // 连接删除按钮的点击事件到槽函数
    connect(confirmDeleteButton, &QPushButton::clicked, this, &deleteUserDialog::deleteUser);

    resize(300,400);
}

deleteUserDialog::~deleteUserDialog() {
    // 清理工作（如果有必要）
}

void deleteUserDialog::deleteUser() {
    // 遍历所有用户项，检查每个项的勾选状态
    bool hasSelected = false; // 标记是否有用户被选中

    for (int i = 0; i < userListWidget->count(); ++i) {
        QListWidgetItem* item = userListWidget->item(i);

        // 检查当前用户项的勾选状态
        if (item->checkState() == Qt::Checked) {
            hasSelected = true; // 如果有用户被勾选
            QString username = item->text();

            // 删除用户
            bool success = system_->getUserManager().removeUser(username.toStdString());
            if (success) {
                QMessageBox::information(this, "成功", "用户删除成功！");
            } else {
                QMessageBox::warning(this, "失败", "用户删除失败！");
            }
        }
    }

    if (!hasSelected) {
        QMessageBox::warning(this, "错误", "请先选择要删除的用户！");
        return;
    }

    system_->saveAllData();  // 保存数据
    this->accept();  // 关闭对话框
}
