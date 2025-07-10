#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QFont>
#include <QTableWidgetItem>
#include <QHeaderView>

#include "dailyOrderSummaryDialog.h"
#include "core/Order.h"

dailyOrderSummaryDialog::dailyOrderSummaryDialog(RestaurantSystem *system, QWidget *parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("今日结算汇总");
    resize(900, 650);  // 增大窗口尺寸
    setupUI();
    generateSummary();
}

dailyOrderSummaryDialog::~dailyOrderSummaryDialog() {}

void dailyOrderSummaryDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建表格控件
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);  // 设置列数：订单编号, 餐桌号, 总金额, 状态, 下单时间
    tableWidget->setHorizontalHeaderLabels({"订单编号", "餐桌号", "总金额", "状态", "下单时间"});
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 不允许编辑
    tableWidget->setWordWrap(false); // 禁用换行

    // 设置表格标题的字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    tableWidget->horizontalHeader()->setFont(headerFont);
    // 设置表格内容的字体
    QFont contentFont("思源宋体 CN", 13, QFont::Normal);
    tableWidget->setFont(contentFont);

    // 设置列宽自动调整
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置行高固定为40
    tableWidget->setRowHeight(0, 40);

    mainLayout->addWidget(tableWidget);

    // 添加总金额标签
    totalLabel = new QLabel("总金额: 0.00元", this);
    QFont labelFont("思源宋体 CN", 13, QFont::Normal);
    totalLabel->setFont(labelFont);
    mainLayout->addWidget(totalLabel);

    // 添加关闭按钮
    closeButton = new QPushButton("关闭", this);
    closeButton->setFont(QFont("思源宋体 CN", 11, QFont::Normal));  // 设置按钮字体
    mainLayout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked, this, &dailyOrderSummaryDialog::onClose);

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void dailyOrderSummaryDialog::generateSummary()
{
    QString today = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    const auto &orders = system_->getOrderManager().getOrders();

    double totalSales = 0.0;
    int row = 0;

    for (const Order &order : orders)
    {
        QString orderTime = QString::fromStdString(order.getOrderTime());
        if (QString::fromStdString(order.getStatus()) == "已结算" && orderTime.startsWith(today))
        {
            tableWidget->insertRow(row);

            // 插入数据并设置居中对齐
            QTableWidgetItem *orderIdItem = new QTableWidgetItem(QString::number(order.getOrderId()));
            orderIdItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, 0, orderIdItem);

            QTableWidgetItem *tableIdItem = new QTableWidgetItem(QString::number(order.getTableId()));
            tableIdItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, 1, tableIdItem);

            QTableWidgetItem *totalPriceItem = new QTableWidgetItem(QString::number(order.getTotalPrice(), 'f', 2) + "元");
            totalPriceItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, 2, totalPriceItem);

            QTableWidgetItem *statusItem = new QTableWidgetItem(QString::fromStdString(order.getStatus()));
            statusItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, 3, statusItem);

            QTableWidgetItem *orderTimeItem = new QTableWidgetItem(orderTime);
            orderTimeItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, 4, orderTimeItem);

            totalSales += order.getTotalPrice();
            row++;
        }
    }

    totalLabel->setText(QString("今日总结算金额: %1元").arg(QString::number(totalSales, 'f', 2)));
}

void dailyOrderSummaryDialog::onClose()
{
    accept();  // 关闭对话框
}
