#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QCheckBox>  // 必须包含 QCheckBox 头文件
#include <vector>

#include "editDishStatusDialog.h"
#include "core/Order.h"

editDishStatusDialog::editDishStatusDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("修改订单状态");
    resize(535, 700);  // 调整窗口尺寸
    setupUI();
    loadOrders();
}

editDishStatusDialog::~editDishStatusDialog()
{
}

void editDishStatusDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 创建表格控件，5 列：订单编号、餐桌ID、菜品名称、数量、选择（勾选框）
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    QStringList headers;
    headers << "订单编号" << "餐桌ID" << "菜品名称" << "数量" << "选择";
    tableWidget->setHorizontalHeaderLabels(headers);

    // 设置标题字体（思源宋体 CN，DemiBold，13号）
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    tableWidget->horizontalHeader()->setFont(headerFont);

    // 设置表格内容字体（思源宋体 CN，Normal，13号）
    QFont contentFont("思源宋体 CN", 13, QFont::Normal);
    tableWidget->setFont(contentFont);

    // 设置表格内容居中对齐
    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 固定列宽：订单编号、餐桌ID、数量、勾选框
    tableWidget->setColumnWidth(0, 80);  // 订单编号列宽
    tableWidget->setColumnWidth(1, 80);  // 餐桌ID列宽
    tableWidget->setColumnWidth(2, 170); // 菜品名称列宽
    tableWidget->setColumnWidth(3, 80);  // 数量列宽
    tableWidget->setColumnWidth(4, 80);  // 勾选框列宽

    // 让“菜品名称”列自动调整宽度
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);  // 菜品名称列宽度自动调整

    // 禁用自动列宽调整
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // 固定行高
    tableWidget->setRowHeight(0, 40);  // 设置每一行的高度为40
    tableWidget->setRowHeight(1, 40);
    tableWidget->setRowHeight(2, 40);

    mainLayout->addWidget(tableWidget);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 底部按钮区域：确认更新和取消
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    submitButton = new QPushButton("确认已出菜", this);
    cancelButton = new QPushButton("取消", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(submitButton, &QPushButton::clicked, this, &editDishStatusDialog::onSubmit);
    connect(cancelButton, &QPushButton::clicked, this, &editDishStatusDialog::reject);
}

void editDishStatusDialog::loadOrders()
{
    // 获取系统中所有订单，筛选状态为"已做"的订单（即厨师制作完成的菜）
    const auto& allOrders = system_->getOrderManager().getOrders();
    std::vector<Order> ordersToEdit;
    for (const auto &order : allOrders)
    {
        if (QString::fromStdString(order.getStatus()) == "已做")
        {
            ordersToEdit.push_back(order);
        }
    }

    tableWidget->setRowCount(static_cast<int>(ordersToEdit.size()));
    for (int row = 0; row < static_cast<int>(ordersToEdit.size()); ++row)
    {
        const Order& order = ordersToEdit[row];

        // 第一列：订单编号，同时把订单编号存储到 UserRole 中，便于后续查找
        QTableWidgetItem* itemOrderId = new QTableWidgetItem(QString::number(order.getOrderId()));
        itemOrderId->setData(Qt::UserRole, order.getOrderId());
        itemOrderId->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        tableWidget->setItem(row, 0, itemOrderId);

        // 第二列：餐桌ID
        QTableWidgetItem* itemTableId = new QTableWidgetItem(QString::number(order.getTableId()));
        itemTableId->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        tableWidget->setItem(row, 1, itemTableId);

        // 第三列：菜品名称
        QTableWidgetItem* itemDishName = new QTableWidgetItem(QString::fromStdString(order.getDishname()));
        itemDishName->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        tableWidget->setItem(row, 2, itemDishName);

        // 第四列：数量
        QTableWidgetItem* itemQty = new QTableWidgetItem(QString::number(order.getQuantity()));
        itemQty->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        tableWidget->setItem(row, 3, itemQty);

        // 第五列：选择（勾选框），默认未勾选
        QWidget* checkWidget = new QWidget();
        QHBoxLayout* checkLayout = new QHBoxLayout();
        QCheckBox* checkBox = new QCheckBox();
        checkLayout->addWidget(checkBox);
        checkLayout->setAlignment(Qt::AlignCenter);  // 确保复选框居中
        checkWidget->setLayout(checkLayout);

        // 设置复选框的大小，使其显示完整
        checkWidget->setMinimumHeight(40);  // 设置最小高度以适应复选框

        tableWidget->setCellWidget(row, 4, checkWidget);  // 使用 QWidget 设置复选框
    }
}

void editDishStatusDialog::onSubmit()
{
    // 遍历表格，统计所有被勾选的订单编号
    QList<int> selectedOrderIds;
    int rowCount = tableWidget->rowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        QWidget* checkWidget = tableWidget->cellWidget(row, 4);
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(checkWidget->layout()->itemAt(0)->widget());
        if (checkBox && checkBox->isChecked())
        {
            QTableWidgetItem* orderItem = tableWidget->item(row, 0);
            if (orderItem)
            {
                int orderId = orderItem->data(Qt::UserRole).toInt();
                selectedOrderIds.append(orderId);
            }
        }
    }

    if (selectedOrderIds.isEmpty())
    {
        QMessageBox::information(this, "提示", "请选择要标记为已出菜的订单！");
        return;
    }

    // 更新订单状态，将选中订单的状态从 "已做" 改为 "已出菜"
    QString updatedOrders;
    std::vector<Order>& orders = const_cast<std::vector<Order>&>(system_->getOrderManager().getOrders());
    for (auto &order : orders)
    {
        if (selectedOrderIds.contains(order.getOrderId()))
        {
            if (QString::fromStdString(order.getStatus()) == "已做")
            {
                order.setStatus("已出菜");
                updatedOrders.append(QString("订单编号 %1 已出菜\n").arg(order.getOrderId()));
            }
        }
    }

    if (!updatedOrders.isEmpty())
    {
        system_->saveAllData();
        QMessageBox::information(this, "订单状态更新", updatedOrders);
    }
    else
    {
        QMessageBox::information(this, "提示", "没有订单状态更新！");
    }
    accept();
}
