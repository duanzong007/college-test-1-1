#include <QVBoxLayout>
#include <QHBoxLayout>  // 用于按钮并排显示
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFont>
#include "dishRankingDialog.h"
#include "core/RestaurantSystem.h"

dishRankingDialog::dishRankingDialog(RestaurantSystem *system, QWidget *parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("菜品与酒水排行榜");
    resize(500, 600);
    setupUI();
}

dishRankingDialog::~dishRankingDialog()
{
    // 无需特殊资源释放
}

void dishRankingDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建按钮并排放置
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    dishesButton = new QPushButton("菜品排行榜", this);
    drinksButton = new QPushButton("酒水排行榜", this);

    // 设置按钮字体
    QFont buttonFont("思源宋体 CN", 13, QFont::Normal);
    dishesButton->setFont(buttonFont);
    drinksButton->setFont(buttonFont);

    // 按钮点击信号槽连接
    connect(dishesButton, &QPushButton::clicked, this, &dishRankingDialog::onDishesButtonClicked);
    connect(drinksButton, &QPushButton::clicked, this, &dishRankingDialog::onDrinksButtonClicked);

    // 将按钮添加到按钮布局
    buttonLayout->addWidget(dishesButton);
    buttonLayout->addWidget(drinksButton);

    mainLayout->addLayout(buttonLayout);  // 将按钮布局添加到主布局

    // 创建堆叠窗口
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    // 创建表格
    dishesWidget = new QWidget(this);
    drinksWidget = new QWidget(this);

    dishesTable = new QTableWidget(dishesWidget);
    drinksTable = new QTableWidget(drinksWidget);

    // 禁用表格的编辑功能
    dishesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑
    drinksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑


    // 将表格添加到widget中
    QVBoxLayout *dishesLayout = new QVBoxLayout(dishesWidget);
    dishesLayout->addWidget(dishesTable);

    QVBoxLayout *drinksLayout = new QVBoxLayout(drinksWidget);
    drinksLayout->addWidget(drinksTable);

    // 将widget添加到stackedWidget
    stackedWidget->addWidget(dishesWidget);
    stackedWidget->addWidget(drinksWidget);

    // 默认显示菜品排行榜
    populateDishesTable();

    setLayout(mainLayout);
}

void dishRankingDialog::populateDishesTable()
{
    const auto &orders = system_->getOrderManager().getOrders();

    // 使用两个 map 分别统计菜品（itemType==1）和酒水（itemType==2）的销量
    std::map<QString, int> dishSales;
    for (const auto &order : orders) {
        if (order.getItemType() == 1) {
            QString dishName = QString::fromStdString(order.getDishname());
            if (dishName.isEmpty()) dishName = "未知";
            dishSales[dishName] += order.getQuantity();
        }
    }

    // 将菜品 map 转换为 vector 并降序排序
    std::vector<std::pair<QString, int>> dishVec(dishSales.begin(), dishSales.end());
    std::sort(dishVec.begin(), dishVec.end(), [](const std::pair<QString, int> &a, const std::pair<QString, int> &b){
        return a.second > b.second;
    });

    // 设置表格
    dishesTable->setRowCount(dishVec.size());
    dishesTable->setColumnCount(2);
    dishesTable->setHorizontalHeaderLabels({"菜品名称", "销量"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    dishesTable->horizontalHeader()->setFont(headerFont);
    // 设置内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    dishesTable->setFont(tableFont);

    int row = 0;
    for (const auto &entry : dishVec) {
        dishesTable->setItem(row, 0, new QTableWidgetItem(entry.first));
        dishesTable->setItem(row, 1, new QTableWidgetItem(QString::number(entry.second)));

        // 设置每个单元格的居中对齐
        for (int col = 0; col < 2; ++col) {
            dishesTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }

        row++;
    }

    // 设置表格属性
    dishesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    dishesTable->setRowHeight(0, 40);
}

void dishRankingDialog::populateDrinksTable()
{
    const auto &orders = system_->getOrderManager().getOrders();

    // 使用一个 map 来统计酒水（itemType==2）的销量
    std::map<QString, int> drinkSales;


    for (const auto &order : orders) {
        if (order.getItemType() == 2) {
            QString drinkName = QString::fromStdString(order.getDishname());
            if (drinkName.isEmpty()) drinkName = "未知";
            drinkSales[drinkName] += order.getQuantity();
        }
    }




    // 将酒水 map 转换为 vector 并降序排序
    std::vector<std::pair<QString, int>> drinkVec(drinkSales.begin(), drinkSales.end());
    std::sort(drinkVec.begin(), drinkVec.end(), [](const std::pair<QString, int> &a, const std::pair<QString, int> &b){
        return a.second > b.second;
    });

    // 设置表格
    drinksTable->setRowCount(drinkVec.size());
    drinksTable->setColumnCount(2);
    drinksTable->setHorizontalHeaderLabels({"酒水名称", "销量"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    drinksTable->horizontalHeader()->setFont(headerFont);
    // 设置内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    drinksTable->setFont(tableFont);

    int row = 0;
    for (const auto &entry : drinkVec) {
        drinksTable->setItem(row, 0, new QTableWidgetItem(entry.first));
        drinksTable->setItem(row, 1, new QTableWidgetItem(QString::number(entry.second)));

        // 设置每个单元格的居中对齐
        for (int col = 0; col < 2; ++col) {
            drinksTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }

        row++;
    }

    // 设置表格属性
    drinksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    drinksTable->setRowHeight(0, 40);
}

void dishRankingDialog::onDishesButtonClicked()
{

    stackedWidget->setCurrentWidget(dishesWidget);  // 切换到菜品排行榜
}

void dishRankingDialog::onDrinksButtonClicked()
{

    populateDrinksTable();  // 只有在点击酒水按钮时填充酒水表格
    stackedWidget->setCurrentWidget(drinksWidget);  // 切换到酒水排行榜
}
