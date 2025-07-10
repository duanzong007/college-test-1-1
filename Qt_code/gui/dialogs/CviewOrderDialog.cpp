#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>

#include "core/Order.h"
#include "core/TableManager.h"
#include "CviewOrderDialog.h"

CviewOrderDialog::CviewOrderDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system), tableId_(0)
{
    setWindowTitle("查看订单");
    resize(900, 800);  // 稍微放大窗口
    setupUI();

    // 根据当前消费者的 ID 获取所属桌号
    int userId = system_->getCurrentUser().getId();
    tableId_ = system_->getTableManager().findTableIdByUser(userId);
    if (tableId_ == 0) {
        QMessageBox::information(this, "提示", "您当前没有预定餐桌！");
        // 若没有预定桌子，也可选择关闭对话框
        close();
        return;
    } else {
        loadOrdersForTable(tableId_);
    }
}

CviewOrderDialog::~CviewOrderDialog()
{
}

void CviewOrderDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 设置统一字体：思源宋体 CN，Normal，13号
    QFont contentFont("思源宋体 CN", 13, QFont::Normal);

    // 创建组框显示菜品订单
    dishGroupBox = new QGroupBox("菜品订单", this);
    dishGroupBox->setFont(contentFont);  // 设置字体
    QVBoxLayout* dishLayout = new QVBoxLayout(dishGroupBox);
    dishTableWidget = new QTableWidget(this);
    // 设置列：名称、数量、总金额、状态、下单时间
    dishTableWidget->setColumnCount(5);
    QStringList dishHeaders;
    dishHeaders << "名称" << "数量" << "总金额" << "状态" << "下单时间";
    dishTableWidget->setHorizontalHeaderLabels(dishHeaders);

    // 设置标题字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    dishTableWidget->horizontalHeader()->setFont(headerFont);
    dishTableWidget->setFont(contentFont);
    dishTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 设置列宽自动调整
    dishTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 自动调整宽度

    // 设置固定行高为 40
    dishTableWidget->setRowHeight(0, 40);  // 设置行高为 40
    dishTableWidget->setRowHeight(1, 40);
    dishTableWidget->setRowHeight(2, 40);

    dishLayout->addWidget(dishTableWidget);

    // 创建组框显示酒水订单
    drinkGroupBox = new QGroupBox("酒水订单", this);
    drinkGroupBox->setFont(contentFont);  // 设置字体
    QVBoxLayout* drinkLayout = new QVBoxLayout(drinkGroupBox);
    drinkTableWidget = new QTableWidget(this);
    drinkTableWidget->setColumnCount(5);
    QStringList drinkHeaders;
    drinkHeaders << "名称" << "数量" << "总金额" << "状态" << "下单时间";
    drinkTableWidget->setHorizontalHeaderLabels(drinkHeaders);

    // 设置标题字体
    drinkTableWidget->horizontalHeader()->setFont(headerFont);
    drinkTableWidget->setFont(contentFont);
    drinkTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 设置列宽自动调整
    drinkTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 自动调整宽度

    // 设置固定行高为 40
    drinkTableWidget->setRowHeight(0, 40);
    drinkTableWidget->setRowHeight(1, 40);
    drinkTableWidget->setRowHeight(2, 40);

    drinkLayout->addWidget(drinkTableWidget);

    mainLayout->addWidget(dishGroupBox);
    mainLayout->addWidget(drinkGroupBox);

    // 底部区域：关闭按钮
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    closeButton = new QPushButton("关闭", this);
    closeButton->setFont(contentFont);  // 设置按钮字体
    bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);
    mainLayout->addLayout(bottomLayout);
    connect(closeButton, &QPushButton::clicked, this, &CviewOrderDialog::accept);


    // 禁用编辑功能
    dishTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    drinkTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void CviewOrderDialog::loadOrdersForTable(int tableId)
{
    // 过滤出该桌子的所有订单
    const auto& allOrders = system_->getOrderManager().getOrders();
    std::vector<Order> tableOrders;
    for (const auto &order : allOrders)
    {
        if (order.getTableId() == tableId && order.getStatus() != "已结算")
            tableOrders.push_back(order);
    }

    // 分成菜品订单（itemType == 1）和酒水订单（itemType == 2）
    std::vector<Order> dishOrders;
    std::vector<Order> drinkOrders;
    for (const auto &order : tableOrders)
    {
        if (order.getItemType() == 1)
            dishOrders.push_back(order);
        else if (order.getItemType() == 2)
            drinkOrders.push_back(order);
    }

    // 更新菜品订单表格
    dishTableWidget->setRowCount(static_cast<int>(dishOrders.size()));
    for (int i = 0; i < static_cast<int>(dishOrders.size()); ++i)
    {
        const Order& order = dishOrders[i];
        // 列1: 名称
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(order.getDishname()));
        itemName->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        dishTableWidget->setItem(i, 0, itemName);
        // 列2: 数量
        QTableWidgetItem* itemQty = new QTableWidgetItem(QString::number(order.getQuantity()));
        itemQty->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        dishTableWidget->setItem(i, 1, itemQty);
        // 列3: 总金额
        QTableWidgetItem* itemTotal = new QTableWidgetItem(QString::number(order.getTotalPrice(), 'f', 2));
        itemTotal->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        dishTableWidget->setItem(i, 2, itemTotal);
        // 列4: 状态
        QTableWidgetItem* itemStatus = new QTableWidgetItem(QString::fromStdString(order.getStatus()));
        itemStatus->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        dishTableWidget->setItem(i, 3, itemStatus);
        // 列5: 下单时间
        QTableWidgetItem* itemTime = new QTableWidgetItem(QString::fromStdString(order.getOrderTime()));
        itemTime->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        dishTableWidget->setItem(i, 4, itemTime);
    }

    // 更新酒水订单表格
    drinkTableWidget->setRowCount(static_cast<int>(drinkOrders.size()));
    for (int i = 0; i < static_cast<int>(drinkOrders.size()); ++i)
    {
        const Order& order = drinkOrders[i];
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(order.getDishname()));
        itemName->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        drinkTableWidget->setItem(i, 0, itemName);
        QTableWidgetItem* itemQty = new QTableWidgetItem(QString::number(order.getQuantity()));
        itemQty->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        drinkTableWidget->setItem(i, 1, itemQty);
        QTableWidgetItem* itemTotal = new QTableWidgetItem(QString::number(order.getTotalPrice(), 'f', 2));
        itemTotal->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        drinkTableWidget->setItem(i, 2, itemTotal);
        QTableWidgetItem* itemStatus = new QTableWidgetItem(QString::fromStdString(order.getStatus()));
        itemStatus->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        drinkTableWidget->setItem(i, 3, itemStatus);
        QTableWidgetItem* itemTime = new QTableWidgetItem(QString::fromStdString(order.getOrderTime()));
        itemTime->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        drinkTableWidget->setItem(i, 4, itemTime);
    }
}
