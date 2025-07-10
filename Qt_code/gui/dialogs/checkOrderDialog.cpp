#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QSet>
#include <algorithm>
#include <QFont>
#include <QTableWidgetItem>

#include "core/Order.h"
#include "core/Table.h"
#include "checkOrderDialog.h"

checkOrderDialog::checkOrderDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("订单结算");
    resize(800, 600);
    setupUI();
    loadTables();
}

checkOrderDialog::~checkOrderDialog()
{
}

void checkOrderDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 设置窗口、按钮和表格的字体
    QFont font("思源宋体 CN", 13);
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);

    setWindowTitle("订单结算");
    resize(850, 650);  // 略微增大窗口尺寸

    // 上部：桌号选择区域
    QHBoxLayout* topLayout = new QHBoxLayout;
    QLabel* tableLabel = new QLabel("选择结算桌号:", this);
    tableLabel->setFont(font);
    tableComboBox = new QComboBox(this);
    tableComboBox->setFont(font);
    connect(tableComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTableChanged(int)));
    topLayout->addWidget(tableLabel);
    topLayout->addWidget(tableComboBox);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // 中部：订单显示表格
    orderTableWidget = new QTableWidget(this);
    orderTableWidget->setColumnCount(6);
    QStringList headers;
    headers << "类型" << "名称" << "数量" << "总金额" << "状态" << "计入结算";
    orderTableWidget->setHorizontalHeaderLabels(headers);

    // 设置表头字体
    orderTableWidget->horizontalHeader()->setFont(headerFont);
    orderTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置行高为40
    orderTableWidget->setRowHeight(0, 40);

    // 确保所有表格内容都居中对齐
    for (int row = 0; row < orderTableWidget->rowCount(); ++row) {
        for (int col = 0; col < orderTableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = orderTableWidget->item(row, col);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter); // 使内容居中
            }
        }
    }

    // 设置表格内容的字体
    orderTableWidget->setFont(font);

    // 连接信号
    connect(orderTableWidget, &QTableWidget::itemChanged, this, &checkOrderDialog::onOrderItemChanged);
    mainLayout->addWidget(orderTableWidget);

    // 显示总金额
    totalLabel = new QLabel("总金额: 0.00元", this);
    totalLabel->setFont(font);
    mainLayout->addWidget(totalLabel);

    // 下部：支付金额输入区域
    QHBoxLayout* payLayout = new QHBoxLayout;
    QLabel* payLabel = new QLabel("顾客支付金额:", this);
    payLabel->setFont(font);
    amountEdit = new QLineEdit(this);
    amountEdit->setFont(font);
    payLayout->addWidget(payLabel);
    payLayout->addWidget(amountEdit);
    payLayout->addStretch();
    mainLayout->addLayout(payLayout);

    // 底部：按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    settleButton = new QPushButton("结算", this);
    settleButton->setFont(font);
    cancelButton = new QPushButton("取消", this);
    cancelButton->setFont(font);
    buttonLayout->addStretch();
    buttonLayout->addWidget(settleButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(settleButton, &QPushButton::clicked, this, &checkOrderDialog::onSettle);
    connect(cancelButton, &QPushButton::clicked, this, &checkOrderDialog::reject);

    // 禁用表格的编辑功能
    orderTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void checkOrderDialog::loadTables()
{
    tableComboBox->clear();
    const auto& orders = system_->getOrderManager().getOrders();
    QSet<int> tableIds;
    for (const auto &order : orders)
    {
        // 只考虑未结算的订单
        if (QString::fromStdString(order.getStatus()) != "已结算")
            tableIds.insert(order.getTableId());
    }
    QList<int> tableList = tableIds.values();
    std::sort(tableList.begin(), tableList.end());
    for (int tid : tableList)
    {
        QString text = QString("桌号 %1").arg(tid);
        tableComboBox->addItem(text, tid);
    }
    if (tableComboBox->count() > 0)
    {
        tableComboBox->setCurrentIndex(0);
        onTableChanged(0);
    }
    else
    {
        orderTableWidget->setRowCount(0);
    }
}
void checkOrderDialog::onTableChanged(int index)
{
    if (index < 0) return;
    int tid = tableComboBox->currentData().toInt();
    // 加载指定桌子的订单（只显示未结算订单）
    const auto& allOrders = system_->getOrderManager().getOrders();
    std::vector<Order> tableOrders;
    for (const auto &order : allOrders)
    {
        if (order.getTableId() == tid && QString::fromStdString(order.getStatus()) != "已结算")
            tableOrders.push_back(order);
    }

    orderTableWidget->blockSignals(true); // 临时屏蔽信号，防止更新时触发 itemChanged
    orderTableWidget->setRowCount(static_cast<int>(tableOrders.size()));
    for (int i = 0; i < static_cast<int>(tableOrders.size()); ++i)
    {
        const Order& order = tableOrders[i];
        // 第一列：类型
        QString typeStr = (order.getItemType() == 1) ? "菜品" : "酒水";
        QTableWidgetItem* itemType = new QTableWidgetItem(typeStr);
        orderTableWidget->setItem(i, 0, itemType);
        itemType->setTextAlignment(Qt::AlignCenter); // 设置居中对齐

        // 第二列：名称
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(order.getDishname()));
        orderTableWidget->setItem(i, 1, itemName);
        itemName->setTextAlignment(Qt::AlignCenter); // 设置居中对齐

        // 第三列：数量
        QTableWidgetItem* itemQty = new QTableWidgetItem(QString::number(order.getQuantity()));
        orderTableWidget->setItem(i, 2, itemQty);
        itemQty->setTextAlignment(Qt::AlignCenter); // 设置居中对齐

        // 第四列：总金额
        QTableWidgetItem* itemTotal = new QTableWidgetItem(QString::number(order.getTotalPrice(), 'f', 2));
        orderTableWidget->setItem(i, 3, itemTotal);
        itemTotal->setTextAlignment(Qt::AlignCenter); // 设置居中对齐

        // 第五列：状态
        QTableWidgetItem* itemStatus = new QTableWidgetItem(QString::fromStdString(order.getStatus()));
        orderTableWidget->setItem(i, 4, itemStatus);
        itemStatus->setTextAlignment(Qt::AlignCenter); // 设置居中对齐

        // 第六列：计入结算（复选框）
        QTableWidgetItem* itemCheck = new QTableWidgetItem();
        itemCheck->setFlags(itemCheck->flags() | Qt::ItemIsUserCheckable);
        // 默认：如果状态是 "已出菜" 自动勾选，否则不勾选
        if(QString::fromStdString(order.getStatus()) == "已出菜")
            itemCheck->setCheckState(Qt::Checked);
        else
            itemCheck->setCheckState(Qt::Unchecked);
        orderTableWidget->setItem(i, 5, itemCheck);
        itemCheck->setTextAlignment(Qt::AlignCenter); // 设置居中对齐
    }
    orderTableWidget->blockSignals(false);

    // 计算并更新总金额显示
    double orderTotal = calculateTotalAmount();
    double finalTotal = orderTotal;
    QString feeInfo;
    if(orderTotal < 500.0)
    {
        finalTotal += 5.0;
        feeInfo = " (包含餐位费5元)";
    }
    else
    {
        feeInfo = " (减免餐位费)";
    }
    totalLabel->setText(QString("总金额: %1元%2").arg(QString::number(finalTotal, 'f', 2)).arg(feeInfo));
}

double checkOrderDialog::calculateTotalAmount() const
{
    double total = 0.0;
    int rowCount = orderTableWidget->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        QTableWidgetItem* checkItem = orderTableWidget->item(i, 5);
        if (checkItem && checkItem->checkState() == Qt::Checked)
        {
            QTableWidgetItem* totalItem = orderTableWidget->item(i, 3);
            if (totalItem)
                total += totalItem->text().toDouble();
        }
    }
    return total;
}

void checkOrderDialog::onOrderItemChanged(QTableWidgetItem* item)
{
    if(item->column() == 5)
    {
        // 重新计算并更新总金额
        double orderTotal = calculateTotalAmount();
        double finalTotal = orderTotal;
        QString feeInfo;
        if(orderTotal < 500.0)
        {
            finalTotal += 5.0;
            feeInfo = " (包含餐位费5元)";
        }
        else
        {
            feeInfo = " (减免餐位费)";
        }
        totalLabel->setText(QString("总金额: %1元%2").arg(QString::number(finalTotal, 'f', 2)).arg(feeInfo));
    }
}

void checkOrderDialog::onSettle()
{
    double orderTotal = calculateTotalAmount();
    double finalTotal = orderTotal;
    if(orderTotal < 500.0)
        finalTotal += 5.0; // 餐位费

    // 提示未勾选的订单（待做或已做）的信息
    int rowCount = orderTableWidget->rowCount();
    QString warningMsg;
    bool hasUnchecked = false;
    for (int i = 0; i < rowCount; ++i)
    {
        QTableWidgetItem* checkItem = orderTableWidget->item(i, 5);
        QTableWidgetItem* statusItem = orderTableWidget->item(i, 4);
        if (checkItem && checkItem->checkState() == Qt::Unchecked)
        {
            QString status = statusItem ? statusItem->text() : "";
            if(status == "待做" || status == "已做")
            {
                hasUnchecked = true;
                warningMsg.append(QString("订单 \"%1\" 将结算金额记为0元\n").arg(orderTableWidget->item(i, 1)->text()));
            }
        }
    }
    if(hasUnchecked)
    {
        QMessageBox::warning(this, "提示", warningMsg + "请确认是否继续结算？");
        // 提示后继续结算
    }

    double paid = amountEdit->text().toDouble();
    if(paid < finalTotal)
    {
        QMessageBox::warning(this, "支付不足", "顾客支付金额不足，无法结算！");
        return;
    }

    double change = paid - finalTotal;

    int currentTable = tableComboBox->currentData().toInt();
    // 更新订单状态：遍历对应桌子的订单
    std::vector<Order>& orders = const_cast<std::vector<Order>&>(system_->getOrderManager().getOrders());
    for (int i = 0; i < rowCount; ++i)
    {
        QTableWidgetItem* checkItem = orderTableWidget->item(i, 5);
        QString dishName = orderTableWidget->item(i, 1)->text();
        for (auto &order : orders)
        {
            if(order.getTableId() == currentTable && QString::fromStdString(order.getDishname()) == dishName)
            {
                if(checkItem && checkItem->checkState() == Qt::Checked)
                {
                    // 计入结算：状态置为 "已结算"，金额不变
                    order.setStatus("已结算");
                }
                else
                {
                    // 未勾选的订单：状态置为 "已结算"，金额设为 0
                    order.setStatus("已结算");
                    order.setTotalPrice(0.0f);
                }
            }
        }
    }

    // 清空桌子状态：将结算后的桌子状态改为 "空闲"，并清空用户ID
    auto& tables = const_cast<std::vector<Table>&>(system_->getTableManager().getTables());
    for (auto &table : tables)
    {
        if (table.getId() == currentTable)
        {
            table.setStatus("空闲");
            table.setUserId(0);
            break;
        }
    }

    system_->saveAllData();
    QMessageBox::information(this, "结算成功", QString("找零金额: %1元").arg(QString::number(change, 'f', 2)));
    accept();
}
