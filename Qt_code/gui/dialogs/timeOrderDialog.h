#ifndef TIMEORDERDIALOG_H
#define TIMEORDERDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class timeOrderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit timeOrderDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~timeOrderDialog();

private:
    RestaurantSystem *system_;
};

#endif // TIMEORDERDIALOG_H
