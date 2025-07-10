#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDateTime>
#include <QMap>
#include <QScrollArea>
#include <QVector>
#include <tuple>

#include "CaddOrderDialog.h"
#include "core/Order.h"

CaddOrderDialog::CaddOrderDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("点餐");
    resize(400, 800);  // 设置窗口大小为400x800
    setupUI();
}

CaddOrderDialog::~CaddOrderDialog()
{
    // 无需特殊释放
}

void CaddOrderDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 设置字体
    QFont contentFont("思源宋体 CN", 13);
    contentFont.setWeight(QFont::Normal);

    // ---------- 点餐区域 ----------
    // 由于消费者不能选择桌号，所以直接展示菜品和酒水的点餐项
    // 创建菜品组
    dishGroupBox = new QGroupBox("菜品", this);
    QVBoxLayout* dishLayout = new QVBoxLayout(dishGroupBox);
    const auto& dishes = system_->getDishManager().getDishes();
    for (const auto &dish : dishes) {
        if (dish.getType() == 1) {  // 菜品
            QHBoxLayout* rowLayout = new QHBoxLayout;
            // 显示格式：ID, 名称
            QString dishText = QString("ID:%1  %2")
                                   .arg(dish.getId())
                                   .arg(QString::fromStdString(dish.getName()));
            QLabel* dishLabel = new QLabel(dishText, this);
            dishLabel->setFont(contentFont);

            // 显示价格，并将 "元" 改为 "¥"（羊字头）
            QLabel* priceLabel = new QLabel(QString("¥%1").arg(dish.getPrice()), this);
            priceLabel->setFont(contentFont);
            priceLabel->setAlignment(Qt::AlignCenter); // 价格居中显示

            // 设置价格列固定宽度，确保与其他列对齐
            priceLabel->setFixedWidth(80);  // 设置价格列的固定宽度

            QSpinBox* spinBox = new QSpinBox(this);
            spinBox->setRange(0, 100);
            spinBox->setValue(0);

            // 添加菜品名称、价格和数量到同一行
            rowLayout->addWidget(dishLabel);
            rowLayout->addStretch();  // 将价格列推到右侧
            rowLayout->addWidget(priceLabel);  // 价格列
            rowLayout->addWidget(spinBox);  // 数量列
            dishLayout->addLayout(rowLayout);
            dishSpinBoxes.insert(dish.getId(), spinBox);
        }
    }

    // 创建酒水组
    drinkGroupBox = new QGroupBox("酒水", this);
    QVBoxLayout* drinkLayout = new QVBoxLayout(drinkGroupBox);
    for (const auto &dish : dishes) {
        if (dish.getType() == 2) {  // 酒水
            QHBoxLayout* rowLayout = new QHBoxLayout;
            QString drinkText = QString("ID:%1  %2")
                                    .arg(dish.getId())
                                    .arg(QString::fromStdString(dish.getName()));
            QLabel* drinkLabel = new QLabel(drinkText, this);
            drinkLabel->setFont(contentFont);

            QLabel* priceLabel = new QLabel(QString("¥%1").arg(dish.getPrice()), this);
            priceLabel->setFont(contentFont);
            priceLabel->setAlignment(Qt::AlignCenter); // 价格居中显示
            priceLabel->setFixedWidth(80);  // 设置价格列的固定宽度

            QSpinBox* spinBox = new QSpinBox(this);
            spinBox->setRange(0, 100);
            spinBox->setValue(0);

            // 添加酒水名称、价格和数量到同一行
            rowLayout->addWidget(drinkLabel);
            rowLayout->addStretch();  // 将价格列推到右侧
            rowLayout->addWidget(priceLabel);  // 价格列
            rowLayout->addWidget(spinBox);  // 数量列
            drinkLayout->addLayout(rowLayout);
            drinkSpinBoxes.insert(dish.getId(), spinBox);
        }
    }

    // 将菜品组和酒水组放入滚动区域
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
    buttonLayout->addStretch();
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // 信号连接
    connect(submitButton, &QPushButton::clicked, this, &CaddOrderDialog::onSubmit);
    connect(cancelButton, &QPushButton::clicked, this, &CaddOrderDialog::reject);
}

void CaddOrderDialog::onSubmit()
{
    // 由消费者身份自动查找所属桌号
    int userId = system_->getCurrentUser().getId();
    int tableId = system_->getTableManager().findTableIdByUser(userId);
    if (tableId == 0) {
        QMessageBox::warning(this, "错误", "您当前没有预定餐桌，无法下单！");
        return;
    }

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
                    ordersToAdd.push_back(std::make_tuple(dish.getId(), qty, dish.getPrice(),
                                                          QString::fromStdString(dish.getName())));
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
                    ordersToAdd.push_back(std::make_tuple(dish.getId(), qty, dish.getPrice(),
                                                          QString::fromStdString(dish.getName())));
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

    // 弹出确认对话框，显示订单明细
    int ret = QMessageBox::question(this, "确认订单", summary, QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes)
        return;

    // 生成当前时间字符串
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 更新桌子状态：如果状态为 "已预定"，则变为 "使用中"
    system_->getTableManager().useTable(tableId);

    // 将订单添加到 OrderManager 中
    std::vector<Order>& orders = const_cast<std::vector<Order>&>(system_->getOrderManager().getOrders());
    int newOrderId = orders.size() + 1;

    // 添加订单
    for (const auto &item : ordersToAdd) {
        int qty = std::get<1>(item);
        float unitPrice = std::get<2>(item);
        QString dishName = std::get<3>(item);
        float subtotal = qty * unitPrice;

        int itemType = 0;
        // 通过菜品名称判断类型
        for (const auto &dish : system_->getDishManager().getDishes()) {
            if (QString::fromStdString(dish.getName()) == dishName) {
                itemType = dish.getType();
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
