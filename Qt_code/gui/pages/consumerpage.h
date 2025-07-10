#ifndef CONSUMERPAGE_H
#define CONSUMERPAGE_H

#include <QWidget>

#include "core/RestaurantSystem.h"

namespace Ui {
class consumerpage;
}

class consumerpage : public QWidget
{
    Q_OBJECT

public:
    explicit consumerpage(RestaurantSystem *system, QWidget *parent = nullptr);
    ~consumerpage();

private:
    Ui::consumerpage *ui;
    RestaurantSystem* system_;  // 后端接口

    void Button_getTableDialog_clicked();
    void Button_CaddOrderDialog_clicked();
    void Button_CviewOrderDialog_clicked();
};

#endif // CONSUMERPAGE_H
