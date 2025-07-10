#ifndef CASHIERPAGE_H
#define CASHIERPAGE_H

#include <QWidget>

#include "core/RestaurantSystem.h"

namespace Ui {
class cashierpage;
}

class cashierpage : public QWidget
{
    Q_OBJECT

public:
    explicit cashierpage(RestaurantSystem *system, QWidget *parent = nullptr);
    ~cashierpage();

private:
    Ui::cashierpage *ui;
    RestaurantSystem* system_;  // 后端接口

    void Button_viewOrderDialog_clicked();
    void Button_checkOrderDialog_clicked();
    void Button_dailyOrderSummaryDialog_clicked();
};

#endif // CASHIERPAGE_H
