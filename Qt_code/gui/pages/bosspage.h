#ifndef BOSSPAGE_H
#define BOSSPAGE_H

#include <QWidget>

#include "core/RestaurantSystem.h"

namespace Ui {
class bosspage;
}

class bosspage : public QWidget
{
    Q_OBJECT

public:
    explicit bosspage(RestaurantSystem *system, QWidget *parent = nullptr);
    ~bosspage();

private:
    Ui::bosspage *ui;
    RestaurantSystem* system_;  // 后端接口
    void Button_viewAllInfo_clicked();
    void Button_viewAllOrder_clicked();
    void Button_dishRankingDialog_clicked();
    void Button_timeOrderDialog_clicked();
};

#endif // BOSSPAGE_H
