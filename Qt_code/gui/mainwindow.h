#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "core/RestaurantSystem.h"
#include "logindialog.h"
#include "gui/pages/adminpage.h"
#include "gui/pages/waiterpage.h"
#include "gui/pages/chefpage.h"
#include "gui/pages/cashierpage.h"
#include "gui/pages/bosspage.h"
#include "gui/pages/consumerpage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void onLogoutButtonClicked();
    ~MainWindow();

private slots:
    void onLoginSuccess(const User &user);

private:
    Ui::MainWindow *ui;
    RestaurantSystem *system_;
    QLabel *roleLabel;
    QPushButton *logoutButton;


    QStackedWidget *stackedWidget_;

    // 各个页面
    logindialog *loginDialog_;
    adminpage *adminPage_;
    waiterpage *waiterPage_;
    chefpage *chefPage_;
    cashierpage *cashierPage_;
    bosspage *bossPage_;
    consumerpage *consumerPage_;


};
#endif // MAINWINDOW_H
