#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

#include "passwordDialog.h"

passwordDialog::passwordDialog(RestaurantSystem *system, QWidget *parent) :
    QDialog(parent), system_(system) {

    setWindowTitle("修改用户密码");

    // 创建布局和控件
    QVBoxLayout* layout = new QVBoxLayout(this);

    userSelectComboBox = new QComboBox(this);
    newPasswordLineEdit = new QLineEdit(this);
    newPasswordLineEdit->setEchoMode(QLineEdit::Password);  // 密码框，输入内容为星号
    saveButton = new QPushButton("保存修改", this);

    // 标签字体设置
    QLabel* label1 = new QLabel("选择用户:", this);
    label1->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    QLabel* label2 = new QLabel("新密码:", this);
    label2->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    // 设置控件字体
    userSelectComboBox->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    newPasswordLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    saveButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);

    layout->addWidget(label1);
    layout->addWidget(userSelectComboBox);
    layout->addWidget(label2);
    layout->addWidget(newPasswordLineEdit);
    layout->addWidget(saveButton);

    // 填充用户选择框
    for (const User& user : system_->getUserManager().getUsers()) {
        userSelectComboBox->addItem(QString::fromStdString(user.getUsername()), QVariant::fromValue(user.getId()));
    }

    // 连接保存按钮的点击事件到槽函数
    connect(saveButton, &QPushButton::clicked, this, &passwordDialog::saveChanges);

    resize(300, 200);

}

passwordDialog::~passwordDialog() {
    // 清理工作（如果有必要）
}

void passwordDialog::saveChanges() {
    // 获取选中的用户ID和新密码
    int selectedUserId = userSelectComboBox->currentData().toInt();
    if (selectedUserId <= 0) {
        QMessageBox::warning(this, "错误", "请选择一个有效的用户！");
        return;
    }

    QString newPassword = newPasswordLineEdit->text();
    if (newPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "密码不能为空！");
        return;
    }

    // 调用 User 类的 setPassword 方法来修改密码
    User& user = system_->getUserManager().getUser(selectedUserId);
    user.setPassword(newPassword.toStdString());  // 修改密码

    // 保存所有数据
    system_->saveAllData();

    QMessageBox::information(this, "成功", "密码修改成功！");
    this->accept();  // 关闭对话框
}
