#include <QMessageBox>

#include "logindialog.h"
#include "core/user.h"
#include "core/restaurantsystem.h"

logindialog::logindialog(RestaurantSystem *system, QWidget *parent)
    : QWidget(parent), ui(new Ui::logindialog), system_(system)
{
    ui->setupUi(this);

    setFixedSize(800,600);

    // 绑定登录按钮
    connect(ui->loginClicked, &QPushButton::clicked, this, &logindialog::onLoginClicked);
    connect(ui->Password, &QLineEdit::returnPressed, this, &logindialog::onLoginClicked);



}

logindialog::~logindialog() {
    delete ui;
}

void logindialog::onLoginClicked() {
    QString username = ui->Account->text();
    QString password = ui->Password->text();

    User authUser;

    if (system_->getUserManager().authenticate(username.toStdString(), password.toStdString(), authUser)) {
        QMessageBox::information(this, "登录成功", "欢迎 " + username);
        system_->setCurrentUser(authUser);
        emit loginSuccess(authUser);  // 通知主窗口
        this->close();

    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}
