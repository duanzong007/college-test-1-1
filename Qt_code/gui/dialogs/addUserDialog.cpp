#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

#include "addUserDialog.h"

addUserDialog::addUserDialog(RestaurantSystem *system, QWidget *parent) :
    QDialog(parent), system_(system) {

    setWindowTitle("添加用户");

    // 创建布局和控件
    QVBoxLayout* layout = new QVBoxLayout(this);
    usernameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);  // 密码框，输入内容为星号

    roleComboBox = new QComboBox(this);
    roleComboBox->addItem("管理员", 1);
    roleComboBox->addItem("服务员", 2);
    roleComboBox->addItem("厨师", 3);
    roleComboBox->addItem("收银员", 4);
    roleComboBox->addItem("老板", 5);
    roleComboBox->addItem("消费者", 6);

    confirmButton = new QPushButton("添加用户", this);

    // 标签字体设置
    QLabel* usernameLabel = new QLabel("用户名：", this);
    usernameLabel->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    QLabel* passwordLabel = new QLabel("密码：", this);
    passwordLabel->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    QLabel* roleLabel = new QLabel("角色：", this);
    roleLabel->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    // 设置控件字体
    usernameLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    passwordLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    roleComboBox->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    usernameLabel->setAlignment(Qt::AlignCenter);
    passwordLabel->setAlignment(Qt::AlignCenter);
    roleLabel->setAlignment(Qt::AlignCenter);

    // 添加控件到布局中
    layout->addWidget(usernameLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(usernameLineEdit, 0, Qt::AlignCenter);
    layout->addWidget(passwordLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(passwordLineEdit, 0, Qt::AlignCenter);
    layout->addWidget(roleLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(roleComboBox, 0, Qt::AlignCenter);
    layout->addWidget(confirmButton, 0, Qt::AlignCenter);

    // 连接按钮的点击事件到槽函数
    connect(confirmButton, &QPushButton::clicked, this, &addUserDialog::addUser);
    resize(300,300);
}

addUserDialog::~addUserDialog() {
    // 清理工作（如果有必要的话）
}

void addUserDialog::addUser() {
    // 获取输入的用户名、密码和角色
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    int role = roleComboBox->currentData().toInt();

    // 检查用户名是否为空
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "输入无效", "用户名和密码不能为空！");
        return;
    }

    // 调用 UserManager 的 addUser 方法添加新用户
    if (system_->getUserManager().addUser(username.toStdString(), password.toStdString(), role)) {
        system_->saveAllData();
        QMessageBox::information(this, "成功", "用户添加成功！");
        this->accept();  // 关闭对话框
    } else {
        QMessageBox::warning(this, "失败", "用户添加失败，用户名可能已存在！");
    }
}
