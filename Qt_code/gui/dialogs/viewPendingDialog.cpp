#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QStringList>
#include <QCheckBox>  // 必须包含 QCheckBox 头文件
#include <algorithm>

#include "viewPendingDialog.h"
#include "core/Order.h"

viewPendingDialog::viewPendingDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("待做订单");
    resize(775, 700);  // 稍微放大窗口尺寸
    setupUI();
    loadPendingOrders();
}

viewPendingDialog::~viewPendingDialog()
{
}

void viewPendingDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 创建订单显示表格
    pendingTableWidget = new QTableWidget(this);
    // 设置6列：类型、名称、数量、总金额、下单时间、完成（复选框）
    pendingTableWidget->setColumnCount(6);
    QStringList headers;
    headers << "类型" << "名称" << "数量" << "总金额" << "下单时间" << "完成";
    pendingTableWidget->setHorizontalHeaderLabels(headers);

    // 设置标题字体（思源宋体 CN，DemiBold，13号）
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    pendingTableWidget->horizontalHeader()->setFont(headerFont);

    // 设置表格内容字体（思源宋体 CN，Normal，13号）
    QFont contentFont("思源宋体 CN", 13, QFont::Normal);
    pendingTableWidget->setFont(contentFont);

    // 设置表格内容居中对齐
    pendingTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 固定列宽：类型、数量、下单时间、完成列
    pendingTableWidget->setColumnWidth(0, 80);  // 类型列宽
    pendingTableWidget->setColumnWidth(1, 170);  // 名称列宽
    pendingTableWidget->setColumnWidth(2, 80);   // 数量列宽
    pendingTableWidget->setColumnWidth(3, 120);  // 总金额列宽
    pendingTableWidget->setColumnWidth(4, 200);  // 下单时间列宽
    pendingTableWidget->setColumnWidth(5, 80);   // 完成列宽


    pendingTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // 禁用自动列宽调整
    pendingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    mainLayout->addWidget(pendingTableWidget);

    // 底部按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    refreshButton = new QPushButton("刷新列表", this);
    closeButton = new QPushButton("关闭", this);

    // 设置按钮字体（思源宋体 CN，Normal，11号）
    QFont buttonFont("思源宋体 CN", 11, QFont::Normal);
    refreshButton->setFont(buttonFont);
    closeButton->setFont(buttonFont);

    buttonLayout->addStretch();
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    connect(refreshButton, &QPushButton::clicked, this, &viewPendingDialog::onRefresh);
    connect(closeButton, &QPushButton::clicked, this, &viewPendingDialog::onClose);
}

void viewPendingDialog::loadPendingOrders()
{
    // 获取所有订单中状态为 "待做" 的订单，并按下单时间升序排序
    const auto &allOrders = system_->getOrderManager().getOrders();
    std::vector<Order> pendingOrders;
    for (const auto &order : allOrders)
    {
        if (QString::fromStdString(order.getStatus()) == "待做")
        {
            pendingOrders.push_back(order);
        }
    }
    std::sort(pendingOrders.begin(), pendingOrders.end(), [](const Order &a, const Order &b) {
        return QString::fromStdString(a.getOrderTime()) < QString::fromStdString(b.getOrderTime());
    });

    pendingTableWidget->setRowCount(static_cast<int>(pendingOrders.size()));
    for (int i = 0; i < static_cast<int>(pendingOrders.size()); ++i)
    {
        const Order &order = pendingOrders[i];
        // 第一列：类型 (菜品/酒水)
        QString typeStr = (order.getItemType() == 1) ? "菜品" : "酒水";
        QTableWidgetItem* itemType = new QTableWidgetItem(typeStr);
        itemType->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        pendingTableWidget->setItem(i, 0, itemType);

        // 第二列：名称
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(order.getDishname()));
        itemName->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        pendingTableWidget->setItem(i, 1, itemName);

        // 第三列：数量
        QTableWidgetItem* itemQty = new QTableWidgetItem(QString::number(order.getQuantity()));
        itemQty->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        pendingTableWidget->setItem(i, 2, itemQty);

        // 第四列：总金额
        QTableWidgetItem* itemTotal = new QTableWidgetItem(QString::number(order.getTotalPrice(), 'f', 2));
        itemTotal->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        pendingTableWidget->setItem(i, 3, itemTotal);

        // 第五列：下单时间
        QTableWidgetItem* itemTime = new QTableWidgetItem(QString::fromStdString(order.getOrderTime()));
        itemTime->setTextAlignment(Qt::AlignCenter);  // 设置单元格内容居中
        pendingTableWidget->setItem(i, 4, itemTime);

        // 第六列：完成复选框
        QTableWidgetItem* itemCheck = new QTableWidgetItem();
        itemCheck->setFlags(itemCheck->flags() | Qt::ItemIsUserCheckable);
        itemCheck->setCheckState(Qt::Unchecked);

        // 使用 QWidget 和 QHBoxLayout 来确保复选框居中
        QWidget* checkWidget = new QWidget();
        QHBoxLayout* checkLayout = new QHBoxLayout();
        QCheckBox* checkBox = new QCheckBox();
        checkLayout->addWidget(checkBox);
        checkLayout->setAlignment(Qt::AlignCenter);  // 确保复选框居中
        checkWidget->setLayout(checkLayout);

        pendingTableWidget->setCellWidget(i, 5, checkWidget);  // 使用 QWidget 设置复选框

        // 设置行高，以确保复选框显示完全
        pendingTableWidget->setRowHeight(i, 40);  // 设置行高为 50，适应复选框
    }
}

void viewPendingDialog::updateCheckedOrders()
{
    // 遍历表格中所有行，检查完成复选框
    int rowCount = pendingTableWidget->rowCount();
    std::vector<Order>& orders = const_cast<std::vector<Order>&>(system_->getOrderManager().getOrders());
    for (int i = 0; i < rowCount; ++i)
    {
        QWidget* checkWidget = pendingTableWidget->cellWidget(i, 5);
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(checkWidget->layout()->itemAt(0)->widget());
        if (checkBox && checkBox->isChecked())
        {
            QString orderName = pendingTableWidget->item(i, 1)->text();
            // 根据订单名称更新系统中状态为 "待做" 的订单为 "已做"
            for (auto &order : orders)
            {
                if (QString::fromStdString(order.getDishname()) == orderName &&
                    QString::fromStdString(order.getStatus()) == "待做")
                {
                    order.setStatus("已做");
                }
            }
        }
    }
    system_->saveAllData();
}

void viewPendingDialog::onRefresh()
{
    updateCheckedOrders();
    loadPendingOrders();
}

void viewPendingDialog::onClose()
{
    updateCheckedOrders();
    system_->saveAllData();
    accept();
}
