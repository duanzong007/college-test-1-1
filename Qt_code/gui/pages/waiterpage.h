#ifndef WAITERPAGE_H
#define WAITERPAGE_H

#include <QWidget>

#include "core/RestaurantSystem.h"

namespace Ui {
class waiterpage;
}

class waiterpage : public QWidget
{
    Q_OBJECT

public:
    explicit waiterpage(RestaurantSystem *system, QWidget *parent = nullptr);
    ~waiterpage();

private:
    Ui::waiterpage *ui;
    RestaurantSystem* system_;  // 后端接口

    void Button_addOrderDialog_clicked();
    void Button_editDishStatusDialog_clicked();
    void Button_viewOrderDialog_clicked();
};

#endif // WAITERPAGE_H
