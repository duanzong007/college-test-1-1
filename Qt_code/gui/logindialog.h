#pragma once

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QQmlEngine>
#include <QWidget>
#include <QMetaType>

#include "core/user.h"
#include "ui_logindialog.h"
#include "core/restaurantsystem.h"

class logindialog : public QWidget
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit logindialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~logindialog();
signals:
    void loginSuccess(User user);  // 登录成功后发送用户对象给主界面

private slots:
    void onLoginClicked();  // 登录按钮槽函数

private:
    Ui::logindialog *ui;
    RestaurantSystem* system_;  // 后端接口
};

#endif
