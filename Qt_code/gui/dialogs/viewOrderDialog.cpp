#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QStringList>
#include <QSet>
#include <algorithm>

#include "viewOrderDialog.h"
#include "core/Order.h"

viewOrderDialog::viewOrderDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("查看小票");
    resize(900, 800);  // 稍微放大窗口
    setupUI();
    loadTableComboBox();
}

viewOrderDialog::~viewOrderDialog()
{
}

void viewOrderDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 设置统一字体：思源宋体 CN，Normal，13号
    QFont contentFont("思源宋体 CN", 13, QFont::Normal);

    // 顶部区域：在右上角显示桌号选择下拉框
    QHBoxLayout* topLayout = new QHBoxLayout;
    QLabel* selectLabel = new QLabel("选择桌号:", this);
    selectLabel->setFont(contentFont);  // 设置字体
    tableComboBox = new QComboBox(this);
    tableComboBox->setFont(contentFont);  // 设置字体
    connect(tableComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTableSelectionChanged(int)));
    topLayout->addWidget(selectLabel);
    topLayout->addWidget(tableComboBox);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // 中间区域：两个组框分别显示菜品和酒水订单
    dishGroupBox = new QGroupBox("菜品订单", this);
    dishGroupBox->setFont(contentFont);  // 设置字体
    QVBoxLayout* dishLayout = new QVBoxLayout(dishGroupBox);
    dishTableWidget = new QTableWidget(this);
    // 列：名称、数量、总金额、状态、下单时间
    dishTableWidget->setColumnCount(5);
    QStringList dishHeaders;
    dishHeaders << "名称" << "数量" << "总金额" << "状态" << "下单时间";
    dishTableWidget->setHorizontalHeaderLabels(dishHeaders);

    // 设置字体和居中对齐
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    dishTableWidget->horizontalHeader()->setFont(headerFont);
    dishTableWidget->setFont(contentFont);
    dishTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 保持列宽自动调整
    dishTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 自动调整宽度

    // 设置固定行高为 40
    dishTableWidget->setRowHeight(0, 40);  // 设置行高为 40
    dishTableWidget->setRowHeight(1, 40);
    dishTableWidget->setRowHeight(2, 40);

    // 禁用编辑功能
    dishTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    dishLayout->addWidget(dishTableWidget);

    drinkGroupBox = new QGroupBox("酒水订单", this);
    drinkGroupBox->setFont(contentFont);  // 设置字体
    QVBoxLayout* drinkLayout = new QVBoxLayout(drinkGroupBox);
    drinkTableWidget = new QTableWidget(this);
    drinkTableWidget->setColumnCount(5);
    QStringList drinkHeaders;
    drinkHeaders << "名称" << "数量" << "总金额" << "状态" << "下单时间";
    drinkTableWidget->setHorizontalHeaderLabels(drinkHeaders);

    // 设置字体和居中对齐
    drinkTableWidget->horizontalHeader()->setFont(headerFont);
    drinkTableWidget->setFont(contentFont);
    drinkTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 保持列宽自动调整
    drinkTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 自动调整宽度

    // 设置固定行高为 40
    drinkTableWidget->setRowHeight(0, 40);
    drinkTableWidget->setRowHeight(1, 40);
    drinkTableWidget->setRowHeight(2, 40);

    // 禁用编辑功能
    drinkTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    connect(closeButton, &QPushButton::clicked, this, &viewOrderDialog::accept);
}

void viewOrderDialog::loadTableComboBox()
{
    tableComboBox->clear();
    const auto& tables = system_->getTableManager().getTables();
    const auto& orders = system_->getOrderManager().getOrders();

    // 选择那些既是 "使用中" 桌子又有订单记录
    QSet<int> tableIds;
    for (const auto &order : orders)
    {
        int tId = order.getTableId();
        // 查找该桌子的状态
        for (const auto &table : tables)
        {
            if (table.getId() == tId && QString::fromStdString(table.getStatus()) == "使用中")
            {
                tableIds.insert(tId);
                break;
            }
        }
    }
    // 使用 values() 方法将 QSet 转为 QList
    QList<int> sortedTableIds = tableIds.values();
    std::sort(sortedTableIds.begin(), sortedTableIds.end());
    for (int tid : sortedTableIds)
    {
        QString text = QString("桌号 %1").arg(tid);
        tableComboBox->addItem(text, tid);
    }
    if (tableComboBox->count() > 0)
    {
        tableComboBox->setCurrentIndex(0);
        loadOrdersForTable(tableComboBox->currentData().toInt());
    }
    else
    {
        dishTableWidget->setRowCount(0);
        drinkTableWidget->setRowCount(0);
    }
}

void viewOrderDialog::onTableSelectionChanged(int index)
{
    if (index < 0)
        return;
    int tableId = tableComboBox->currentData().toInt();
    loadOrdersForTable(tableId);
}

void viewOrderDialog::loadOrdersForTable(int tableId)
{
    // 过滤出该桌子的所有订单
    const auto& allOrders = system_->getOrderManager().getOrders();
    std::vector<Order> tableOrders;
    for (const auto &order : allOrders)
    {
        if (order.getTableId() == tableId && order.getStatus() != "已结算")
            tableOrders.push_back(order);
    }

    // 将订单分为菜品订单（itemType==1）和酒水订单（itemType==2）
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
