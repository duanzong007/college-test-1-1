#ifndef DISHRANKINGDIALOG_H
#define DISHRANKINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QStackedWidget>

#include "core/RestaurantSystem.h"

class dishRankingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit dishRankingDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~dishRankingDialog();

private slots:
    void onDishesButtonClicked();
    void onDrinksButtonClicked();

private:
    void setupUI();
    void populateDishesTable();
    void populateDrinksTable();

    RestaurantSystem *system_;
    QTableWidget *dishesTable;
    QTableWidget *drinksTable;
    QWidget *dishesWidget;
    QWidget *drinksWidget;
    QStackedWidget *stackedWidget;
    QPushButton *dishesButton;
    QPushButton *drinksButton;
};

#endif // DISHRANKINGDIALOG_H
