#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QCheckBox>  // 必须包含 QCheckBox 头文件
#include <sstream>
#include <iomanip>  // 用于 std::setw 和 std::setfill

#include "dailyDishSummaryDialog.h"

dailyDishSummaryDialog::dailyDishSummaryDialog(RestaurantSystem *system, QWidget *parent) :
    QDialog(parent),
    system_(system)
{
    // 设置对话框的基本布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    resize(350,650);

    // 创建 QTableWidget 并设置列数和列头
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels({"菜品/酒水", "数量"});

    // 设置标题字体（思源宋体 CN，DemiBold，13号）
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    tableWidget->horizontalHeader()->setFont(headerFont);

    // 设置表格内容字体（思源宋体 CN，Normal，13号）
    QFont contentFont("思源宋体 CN", 13, QFont::Normal);
    tableWidget->setFont(contentFont);

    // 设置表格内容居中对齐
    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 设置列宽
    tableWidget->setColumnWidth(0, 170);  // 菜品/酒水列宽
    tableWidget->setColumnWidth(1, 80);   // 数量列宽

    // 设置行高固定为40
    tableWidget->setRowHeight(0, 40);  // 设置每一行的高度为40
    tableWidget->setRowHeight(1, 40);
    tableWidget->setRowHeight(2, 40);

    // 填充表格
    populateTable();

    // 设置表格自适应内容
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 将表格添加到布局中
    layout->addWidget(tableWidget);

    setLayout(layout);
    setWindowTitle("当天菜品制作统计");
}

dailyDishSummaryDialog::~dailyDishSummaryDialog()
{
}

void dailyDishSummaryDialog::populateTable()
{
    // 统计当天菜品和酒水的数量
    std::map<std::string, int> dishCountMap;
    std::map<std::string, int> drinkCountMap;

    const auto &orders = system_->getOrderManager().getOrders();

    // 获取今天的日期
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "-"
       << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    std::string todayDate = ss.str();

    // 遍历订单并统计数量
    for (const auto &order : orders)
    {
        if (order.getStatus() != "待做" && order.getOrderTime().substr(0, 10) == todayDate)
        {
            if (order.getItemType() == 1) // 菜品
            {
                dishCountMap[order.getDishname()] += order.getQuantity();
            }
            else if (order.getItemType() == 2) // 酒水
            {
                drinkCountMap[order.getDishname()] += order.getQuantity();
            }
        }
    }

    // 填充菜品统计
    int row = 0;
    for (const auto &entry : dishCountMap)
    {
        tableWidget->insertRow(row);
        QTableWidgetItem* itemDishName = new QTableWidgetItem(QString::fromStdString(entry.first));
        itemDishName->setTextAlignment(Qt::AlignCenter);  // 确保居中对齐
        tableWidget->setItem(row, 0, itemDishName);

        QTableWidgetItem* itemQty = new QTableWidgetItem(QString::number(entry.second));
        itemQty->setTextAlignment(Qt::AlignCenter);  // 确保居中对齐
        tableWidget->setItem(row, 1, itemQty);
        row++;
    }

    // 填充酒水统计
    for (const auto &entry : drinkCountMap)
    {
        tableWidget->insertRow(row);
        QTableWidgetItem* itemDishName = new QTableWidgetItem(QString::fromStdString(entry.first));
        itemDishName->setTextAlignment(Qt::AlignCenter);  // 确保居中对齐
        tableWidget->setItem(row, 0, itemDishName);

        QTableWidgetItem* itemQty = new QTableWidgetItem(QString::number(entry.second));
        itemQty->setTextAlignment(Qt::AlignCenter);  // 确保居中对齐
        tableWidget->setItem(row, 1, itemQty);
        row++;
    }
}
