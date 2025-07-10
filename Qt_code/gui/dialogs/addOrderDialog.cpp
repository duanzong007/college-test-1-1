#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDateTime>
#include <QMap>
#include <QScrollArea>
#include <QVector>
#include <tuple>

#include "addOrderDialog.h"
#include "core/Order.h"

addOrderDialog::addOrderDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("点餐");
    resize(800, 600);
    setupUI();
}

addOrderDialog::~addOrderDialog()
{
    // 无需特殊释放
}

void addOrderDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 设置窗口大小
    resize(400, 800);  // 设置窗口大小为400x800

    // ---------- 桌号选择区域 ----------
    QHBoxLayout* tableLayout = new QHBoxLayout;
    QLabel* tableLabel = new QLabel("选择桌号:", this);

    // 设置正文部分的字体为思源宋体 CN，Normal，字号为13
    QFont contentFont("思源宋体 CN", 13);
    contentFont.setWeight(QFont::Normal); // 设置 Normal 权重
    tableLabel->setFont(contentFont);

    tableComboBox = new QComboBox(this);
    tableComboBox->setFont(contentFont);  // 同样为QComboBox设置思源宋体 CN Normal字体
    const auto& tables = system_->getTableManager().getTables();
    for (const auto &table : tables) {
        std::string status = table.getStatus();
        if (status == "已预定" || status == "使用中") {
            QString itemText = QString("桌号 %1 (%2)")
                                   .arg(table.getId())
                                   .arg(QString::fromStdString(status));
            tableComboBox->addItem(itemText, table.getId());
        }
    }
    tableLayout->addWidget(tableLabel);
    tableLayout->addWidget(tableComboBox);
    mainLayout->addLayout(tableLayout);

    // ---------- 菜品和酒水显示区域 ----------
    dishGroupBox = new QGroupBox("菜品", this);
    QVBoxLayout* dishLayout = new QVBoxLayout(dishGroupBox);
    const auto& dishes = system_->getDishManager().getDishes();
    for (const auto &dish : dishes) {
        if (dish.getType() == 1) {  // 菜品
            QHBoxLayout* rowLayout = new QHBoxLayout;

            // 菜品名称
            QString dishText = QString("ID:%1  %2")
                                   .arg(dish.getId())
                                   .arg(QString::fromStdString(dish.getName()));

            QLabel* dishLabel = new QLabel(dishText, this);
            dishLabel->setFont(contentFont);  // 设置菜品名称的字体为思源宋体 CN Normal，字号13

            // 显示价格
            QLabel* priceLabel = new QLabel(QString("¥%1").arg(dish.getPrice()), this);
            priceLabel->setFont(contentFont);  // 设置价格的字体为思源宋体 CN Normal，字号13

            QSpinBox* spinBox = new QSpinBox(this);
            spinBox->setRange(0, 100);
            spinBox->setValue(0);

            // 设置固定宽度，确保价格列对齐
            priceLabel->setFixedWidth(60);  // 设置价格列的固定宽度，确保对齐

            // 添加菜品名称、价格和数量到同一行
            rowLayout->addWidget(dishLabel);
            rowLayout->addStretch();  // 使价格列独立
            rowLayout->addWidget(priceLabel);  // 价格列
            rowLayout->addWidget(spinBox);  // 数量列
            dishLayout->addLayout(rowLayout);
            dishSpinBoxes.insert(dish.getId(), spinBox);
        }
    }

    drinkGroupBox = new QGroupBox("酒水", this);
    QVBoxLayout* drinkLayout = new QVBoxLayout(drinkGroupBox);
    for (const auto &dish : dishes) {
        if (dish.getType() == 2) {  // 酒水
            QHBoxLayout* rowLayout = new QHBoxLayout;

            // 酒水名称
            QString drinkText = QString("ID:%1  %2")
                                    .arg(dish.getId())
                                    .arg(QString::fromStdString(dish.getName()));

            QLabel* drinkLabel = new QLabel(drinkText, this);
            drinkLabel->setFont(contentFont);  // 设置酒水名称的字体为思源宋体 CN Normal，字号13

            // 显示价格
            QLabel* priceLabel = new QLabel(QString("¥%1").arg(dish.getPrice()), this);
            priceLabel->setFont(contentFont);  // 设置价格的字体为思源宋体 CN Normal，字号13

            QSpinBox* spinBox = new QSpinBox(this);
            spinBox->setRange(0, 100);
            spinBox->setValue(0);

            // 设置固定宽度，确保价格列对齐
            priceLabel->setFixedWidth(60);  // 设置价格列的固定宽度，确保对齐

            // 添加酒水名称、价格和数量到同一行
            rowLayout->addWidget(drinkLabel);
            rowLayout->addStretch();  // 使价格列独立
            rowLayout->addWidget(priceLabel);  // 价格列
            rowLayout->addWidget(spinBox);  // 数量列
            drinkLayout->addLayout(rowLayout);
            drinkSpinBoxes.insert(dish.getId(), spinBox);
        }
    }

    QWidget* orderItemWidget = new QWidget(this);
    QVBoxLayout* orderItemLayout = new QVBoxLayout(orderItemWidget);
    orderItemLayout->setContentsMargins(0, 0, 0, 0);
    orderItemLayout->addWidget(dishGroupBox);
    orderItemLayout->addWidget(drinkGroupBox);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(orderItemWidget);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    // ---------- 按钮区域 ----------
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    submitButton = new QPushButton("下单", this);
    cancelButton = new QPushButton("取消", this);

    // 设置按钮部分的字体为思源宋体 CN，Normal，字号为11
    QFont buttonFont("思源宋体 CN", 11);
    buttonFont.setWeight(QFont::Normal); // 设置 Normal 权重
    submitButton->setFont(buttonFont); // 按钮字体设置为思源宋体 CN Normal
    cancelButton->setFont(buttonFont);  // 按钮字体设置为思源宋体 CN Normal

    buttonLayout->addStretch(); // 添加伸缩项，推按钮到右侧
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(submitButton, &QPushButton::clicked, this, &addOrderDialog::onSubmit);
    connect(cancelButton, &QPushButton::clicked, this, &addOrderDialog::reject);
}


void addOrderDialog::onSubmit()
{
    // 判断是否有可选桌号（此处 tableComboBox 已只显示非空闲桌子）
    if (tableComboBox->currentIndex() < 0) {
        QMessageBox::warning(this, "错误", "请先选择桌号！");
        return;
    }
    int tableId = tableComboBox->currentData().toInt();

    // 汇总点餐信息
    QString summary;
    float totalAmount = 0.0f;
    QVector<std::tuple<int, int, float, QString>> ordersToAdd;  // 每项：菜品ID, 数量, 单价, 名称

    // 处理菜品部分
    for (auto it = dishSpinBoxes.begin(); it != dishSpinBoxes.end(); ++it) {
        int qty = it.value()->value();
        if (qty > 0) {
            for (const auto &dish : system_->getDishManager().getDishes()) {
                if (dish.getType() == 1 && dish.getId() == it.key()) {  // 菜品
                    float subtotal = dish.getPrice() * qty;
                    totalAmount += subtotal;
                    summary.append(QString("菜品: %1, 数量: %2, 小计: %3元\n")
                                       .arg(QString::fromStdString(dish.getName()))
                                       .arg(qty)
                                       .arg(QString::number(subtotal, 'f', 2)));
                    ordersToAdd.push_back(std::make_tuple(dish.getId(), qty, dish.getPrice(), QString::fromStdString(dish.getName())));
                    break;
                }
            }
        }
    }

    // 处理酒水部分
    for (auto it = drinkSpinBoxes.begin(); it != drinkSpinBoxes.end(); ++it) {
        int qty = it.value()->value();
        if (qty > 0) {
            for (const auto &dish : system_->getDishManager().getDishes()) {
                if (dish.getType() == 2 && dish.getId() == it.key()) {  // 酒水
                    float subtotal = dish.getPrice() * qty;
                    totalAmount += subtotal;
                    summary.append(QString("酒水: %1, 数量: %2, 小计: %3元\n")
                                       .arg(QString::fromStdString(dish.getName()))
                                       .arg(qty)
                                       .arg(QString::number(subtotal, 'f', 2)));
                    ordersToAdd.push_back(std::make_tuple(dish.getId(), qty, dish.getPrice(), QString::fromStdString(dish.getName())));
                    break;
                }
            }
        }
    }

    if (ordersToAdd.isEmpty()) {
        QMessageBox::information(this, "提示", "您没有点任何菜品或酒水！");
        return;
    }
    summary.append(QString("\n总金额: %1元").arg(QString::number(totalAmount, 'f', 2)));

    // 使用原来的确认对话框显示订单详情（确认对话框不做滚动区域改变）
    int ret = QMessageBox::question(this, "确认订单", summary, QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes)
        return;

    // 生成当前时间字符串
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 更新桌子状态：如果状态为 "已预定"，则变为 "使用中"
    system_->getTableManager().useTable(tableId);

    // 将订单添加到 OrderManager 中（通过 const_cast 获取可修改订单 vector）
    std::vector<Order>& orders = const_cast<std::vector<Order>&>(system_->getOrderManager().getOrders());
    int newOrderId = orders.size() + 1;

    for (const auto &item : ordersToAdd) {
        int qty = std::get<1>(item);
        float unitPrice = std::get<2>(item);
        QString dishName = std::get<3>(item);
        float subtotal = qty * unitPrice;

        int itemType = 0;
        // 这里根据 dishName 判断菜品类型（1: 菜品，2: 酒水）
        for (const auto &dish : system_->getDishManager().getDishes()) {
            if (QString::fromStdString(dish.getName()) == dishName) {
                itemType = dish.getType();  // 获取菜品或酒水的类型
                break;
            }
        }

        std::string dishNameStr = dishName.toStdString();
        Order newOrder(newOrderId, tableId, itemType, dishNameStr, qty, subtotal, "待做", currentTime.toStdString());
        orders.push_back(newOrder);
        newOrderId++;
    }

    // 排序订单（按订单ID降序）
    std::sort(orders.begin(), orders.end(), [](const Order &a, const Order &b) {
        return a.getOrderId() > b.getOrderId();
    });
    system_->saveAllData();
    QMessageBox::information(this, "下单成功", "订单已成功添加！");
    accept();
}
