#include <QVBoxLayout>
#include <QHBoxLayout>  // 用于按钮并排显示
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFont>
#include "viewAllOrderDialog.h"
#include "core/RestaurantSystem.h"

viewAllOrderDialog::viewAllOrderDialog(RestaurantSystem *system, QWidget *parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("流水明细");
    resize(965, 700);
    setupUI();
}

viewAllOrderDialog::~viewAllOrderDialog()
{
    // 无需特殊释放
}

void viewAllOrderDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建表格
    orderTable = new QTableWidget(this);
    orderTable->setColumnCount(8);  // 设置列数
    orderTable->setHorizontalHeaderLabels({"订单编号", "餐桌ID", "类型", "名称", "数量", "总金额", "状态", "时间"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    orderTable->horizontalHeader()->setFont(headerFont);

    // 设置内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    orderTable->setFont(tableFont);

    // 设置表格行高
    orderTable->setRowHeight(0, 40);


    orderTable->setColumnWidth(0, 80);  // 订单编号
    orderTable->setColumnWidth(1, 80);  // 餐桌ID
    orderTable->setColumnWidth(2, 80);  // 类型
    orderTable->setColumnWidth(3, 170);   // 名称
    orderTable->setColumnWidth(4, 80);   // 数量
    orderTable->setColumnWidth(5, 120);  // 总金额
    orderTable->setColumnWidth(6, 80);  // 状态
    orderTable->setColumnWidth(7, 200);  //时间

    orderTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);  // 禁用自动列宽
    // 禁用表格的编辑功能
    orderTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑

    // 填充表格数据
    populateOrderTable();

    // 添加表格到布局
    mainLayout->addWidget(orderTable);

    // 创建关闭按钮
    closeButton = new QPushButton("关闭", this);
    closeButton->setFont(QFont("思源宋体 CN", 11, QFont::Normal));  // 设置关闭按钮字体
    connect(closeButton, &QPushButton::clicked, this, &viewAllOrderDialog::onCloseButtonClicked);

    // 将关闭按钮放在右下角
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void viewAllOrderDialog::populateOrderTable()
{
    const auto &orders = system_->getOrderManager().getOrders();

    // 遍历所有订单并填充表格
    orderTable->setRowCount(orders.size());

    int row = 0;
    for (const auto &order : orders) {
        QString dishName = QString::fromStdString(order.getDishname());
        if (dishName.isEmpty()) dishName = "未知";

        // 填充每一行的内容
        orderTable->setItem(row, 0, new QTableWidgetItem(QString::number(order.getOrderId())));
        orderTable->setItem(row, 1, new QTableWidgetItem(QString::number(order.getTableId())));
        orderTable->setItem(row, 2, new QTableWidgetItem(order.getItemType() == 1 ? "菜品" : "酒水"));
        orderTable->setItem(row, 3, new QTableWidgetItem(dishName));
        orderTable->setItem(row, 4, new QTableWidgetItem(QString::number(order.getQuantity())));
        orderTable->setItem(row, 5, new QTableWidgetItem(QString::number(order.getTotalPrice(), 'f', 2)));
        orderTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(order.getStatus())));
        orderTable->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(order.getOrderTime())));

        // 设置每个单元格的居中对齐
        for (int col = 0; col < 8; ++col) {
            orderTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }

        row++;
    }
}

void viewAllOrderDialog::onCloseButtonClicked()
{
    this->accept();  // 关闭对话框
}
