#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QWidget>

#include "core/RestaurantSystem.h"

namespace Ui {
class adminpage;
}

class adminpage : public QWidget
{
    Q_OBJECT

public:
    explicit adminpage(RestaurantSystem *system, QWidget *parent = nullptr);
    ~adminpage();

private slots:



private:
    Ui::adminpage *ui;
    RestaurantSystem* system_;  // 后端接口
    void Button_setTableDialog_clicked();
    void Button_setDishDialog_clicked();
    void Button_setDrinkDialog_clicked();
    void Button_addUserDialog_clicked();
    void Button_deleteUserDialog_clicked();
    void Button_passwordDialog_clicked();

};

#endif // ADMINPAGE_H
