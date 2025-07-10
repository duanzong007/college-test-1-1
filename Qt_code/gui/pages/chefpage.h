#ifndef CHEFPAGE_H
#define CHEFPAGE_H

#include <QWidget>

#include "core/RestaurantSystem.h"

namespace Ui {
class chefpage;
}

class chefpage : public QWidget
{
    Q_OBJECT

public:
    explicit chefpage(RestaurantSystem *system, QWidget *parent = nullptr);
    ~chefpage();

private:
    Ui::chefpage *ui;
    RestaurantSystem* system_;  // 后端接口

    void Button_viewPendingDialog_clicked();
    void Button_dailyDishSummaryDialog_clicked();
};

#endif // CHEFPAGE_H
