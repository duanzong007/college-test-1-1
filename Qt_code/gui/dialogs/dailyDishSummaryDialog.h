#ifndef DAILYDISHSUMMARYDIALOG_H
#define DAILYDISHSUMMARYDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <map>

#include "core/RestaurantSystem.h"

namespace Ui {
class dailyDishSummaryDialog;
}

class dailyDishSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dailyDishSummaryDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~dailyDishSummaryDialog();

private:
    Ui::dailyDishSummaryDialog *ui;
    RestaurantSystem *system_; // 用于访问系统中的数据
    QTableWidget *tableWidget; // 声明 QTableWidget 作为成员变量

    void populateTable(); // 填充表格
};

#endif // DAILYDISHSUMMARYDIALOG_H
