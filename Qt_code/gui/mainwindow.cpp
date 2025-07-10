#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1000,600);

    // // 创建背景图
    // QPixmap background(":/png/login.png");
    // QPalette palette;
    // palette.setBrush(QPalette::Window, QBrush(background));
    // this->setPalette(palette);

    // // 设置其他控件背景透明，以避免被背景图覆盖
    // setStyleSheet("QPushButton {""background-color: transparent;""}");



    roleLabel = new QLabel(this);
    roleLabel->setText("未登录");
    roleLabel->setVisible(false);

    logoutButton = new QPushButton(this);
    logoutButton->setText("退出登录");
    logoutButton->setVisible(false);

    ui->statusbar->addPermanentWidget(roleLabel);
    ui->statusbar->addPermanentWidget(logoutButton);

    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);

    system_ = new RestaurantSystem();

    system_->loadAllData();


    stackedWidget_ = new QStackedWidget(this);

    loginDialog_  = new logindialog(system_, stackedWidget_);
    adminPage_    = new adminpage(system_, stackedWidget_);
    waiterPage_   = new waiterpage(system_, stackedWidget_);
    chefPage_     = new chefpage(system_, stackedWidget_);
    cashierPage_  = new cashierpage(system_, stackedWidget_);
    bossPage_     = new bosspage(system_, stackedWidget_);
    consumerPage_ = new consumerpage(system_, stackedWidget_);

    stackedWidget_->addWidget(loginDialog_);
    stackedWidget_->addWidget(adminPage_);
    stackedWidget_->addWidget(waiterPage_);
    stackedWidget_->addWidget(chefPage_);
    stackedWidget_->addWidget(cashierPage_);
    stackedWidget_->addWidget(bossPage_);
    stackedWidget_->addWidget(consumerPage_);




    setCentralWidget(stackedWidget_);
    stackedWidget_->setCurrentWidget(loginDialog_);

    // 连接登录成功信号
    connect(loginDialog_, &logindialog::loginSuccess, this, &MainWindow::onLoginSuccess);




}

void MainWindow::onLoginSuccess(const User &user) {
    int role = user.getRole();
    QString roleStr;
    switch (role) {
    case 1: roleStr = "管理员 " + QString::fromStdString(user.getUsername()); break;
    case 2: roleStr = "服务员 " + QString::fromStdString(user.getUsername()); break;
    case 3: roleStr = "厨师 " + QString::fromStdString(user.getUsername()); break;
    case 4: roleStr = "收银员 " + QString::fromStdString(user.getUsername()); break;
    case 5: roleStr = "老板 " + QString::fromStdString(user.getUsername()); break;
    case 6: roleStr = "消费者 " + QString::fromStdString(user.getUsername()); break;
    default: roleStr = "未知角色 " + QString::fromStdString(user.getUsername()); break;
    }
    roleLabel->setText("当前角色：" + roleStr);
    roleLabel->setVisible(true);
    logoutButton->setVisible(true);


    switch (role) {
    case 1: // 管理员
        stackedWidget_->setCurrentWidget(adminPage_);
        break;
    case 2: // 服务员
        stackedWidget_->setCurrentWidget(waiterPage_);
        break;
    case 3: // 厨师
        stackedWidget_->setCurrentWidget(chefPage_);
        break;
    case 4: // 收银员
        stackedWidget_->setCurrentWidget(cashierPage_);
        break;
    case 5: // 老板
        stackedWidget_->setCurrentWidget(bossPage_);
        break;
    case 6: // 消费者
        stackedWidget_->setCurrentWidget(consumerPage_);
        break;
    default:
        QMessageBox::warning(this, "登录失败", "用户未注册");
        break;
    }
}

void MainWindow::onLogoutButtonClicked()
{
    // 恢复到未登录状态，比如切回登录页面
    roleLabel->setVisible(false);
    logoutButton->setVisible(false);
    roleLabel->setText("未登录");

    // 切换到 loginDialog_ 界面
    stackedWidget_->setCurrentWidget(loginDialog_);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete system_;
}
