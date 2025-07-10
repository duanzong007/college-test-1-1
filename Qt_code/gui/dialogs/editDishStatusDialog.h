#ifndef EDITDISHSTATUSDIALOG_H
#define EDITDISHSTATUSDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class QTableWidget;
class QPushButton;

class editDishStatusDialog : public QDialog
{
    Q_OBJECT
public:
    explicit editDishStatusDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~editDishStatusDialog();

private slots:
    void onSubmit();

private:
    void setupUI();
    void loadOrders();

    RestaurantSystem* system_;
    QTableWidget* tableWidget;
    QPushButton* submitButton;
    QPushButton* cancelButton;
};

#endif // EDITDISHSTATUSDIALOG_H
