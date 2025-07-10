#include <QVBoxLayout>
#include <QHBoxLayout>  // 用于按钮并排显示
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QHeaderView>
#include <QLabel>  // 用于显示餐桌信息和菜品信息
#include <QFont>
#include "viewAllInfoDialog.h"
#include "core/RestaurantSystem.h"

viewAllInfoDialog::viewAllInfoDialog(RestaurantSystem *system, QWidget *parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("系统全部信息");
    resize(900, 600);
    setupUI();
}

viewAllInfoDialog::~viewAllInfoDialog()
{
    // 无需特殊处理
}

void viewAllInfoDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建按钮，并排放置
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    usersButton = new QPushButton("所有用户信息", this);
    tablesButton = new QPushButton("所有餐桌信息", this);
    dishesButton = new QPushButton("所有菜品信息", this);
    drinksButton = new QPushButton("所有酒水信息", this);

    // 设置按钮字体
    QFont buttonFont("思源宋体 CN", 13, QFont::Normal);
    usersButton->setFont(buttonFont);
    tablesButton->setFont(buttonFont);
    dishesButton->setFont(buttonFont);
    drinksButton->setFont(buttonFont);

    // 按钮点击信号槽连接
    connect(usersButton, &QPushButton::clicked, this, &viewAllInfoDialog::onUsersButtonClicked);
    connect(tablesButton, &QPushButton::clicked, this, &viewAllInfoDialog::onTablesButtonClicked);
    connect(dishesButton, &QPushButton::clicked, this, &viewAllInfoDialog::onDishesButtonClicked);
    connect(drinksButton, &QPushButton::clicked, this, &viewAllInfoDialog::onDrinksButtonClicked);

    // 将按钮添加到按钮布局
    buttonLayout->addWidget(usersButton);
    buttonLayout->addWidget(tablesButton);
    buttonLayout->addWidget(dishesButton);
    buttonLayout->addWidget(drinksButton);  // 添加酒水按钮

    mainLayout->addLayout(buttonLayout);  // 将按钮布局添加到主布局

    // 创建堆叠窗口
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    // 创建表格
    usersTable = new QTableWidget(this);
    tablesTable = new QTableWidget(this);
    dishesTable = new QTableWidget(this);
    drinksTable = new QTableWidget(this);  // 新增酒水表格

    // 将表格添加到堆叠窗口
    stackedWidget->addWidget(usersTable);
    stackedWidget->addWidget(tablesTable);
    stackedWidget->addWidget(dishesTable);
    stackedWidget->addWidget(drinksTable);  // 添加酒水表格到堆叠窗口

    // 初始化第一页（用户信息表格）
    populateUsersTable();

    // 添加关闭按钮
    closeButton = new QPushButton("关闭", this);
    closeButton->setFont(QFont("思源宋体 CN", 11, QFont::Normal));  // 设置按钮字体
    connect(closeButton, &QPushButton::clicked, this, &viewAllInfoDialog::onCloseButtonClicked);
    mainLayout->addWidget(closeButton, 0, Qt::AlignRight);  // 将关闭按钮添加到右下角

    setLayout(mainLayout);
}

void viewAllInfoDialog::populateUsersTable()
{
    const auto &users = system_->getUserManager().getUsers();
    usersTable->setRowCount(users.size());
    usersTable->setColumnCount(3);
    usersTable->setHorizontalHeaderLabels({"ID", "用户名", "角色"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    usersTable->horizontalHeader()->setFont(headerFont);
    // 设置表格内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    usersTable->setFont(tableFont);

    int row = 0;
    for (const auto &user : users) {
        usersTable->setItem(row, 0, new QTableWidgetItem(QString::number(user.getId())));
        usersTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(user.getUsername())));

        // 根据角色代码显示对应的角色名
        QString role;
        switch (user.getRole()) {
        case 1: role = "管理员"; break;
        case 2: role = "服务员"; break;
        case 3: role = "厨师"; break;
        case 4: role = "收银员"; break;
        case 5: role = "老板"; break;
        case 6: role = "消费者"; break;
        default: role = "未知角色"; break;
        }
        usersTable->setItem(row, 2, new QTableWidgetItem(role));

        // 设置每个单元格的居中对齐
        for (int col = 0; col < 3; ++col) {
            usersTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }

        row++;
    }

    // 设置居中对齐和行高
    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    usersTable->setRowHeight(0, 40);

    // 禁用表格的编辑功能
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑

}

void viewAllInfoDialog::populateTablesTable()
{
    const auto &tables = system_->getTableManager().getTables();
    tablesTable->setRowCount(tables.size());
    tablesTable->setColumnCount(3);
    tablesTable->setHorizontalHeaderLabels({"餐桌ID", "用户ID", "状态"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    tablesTable->horizontalHeader()->setFont(headerFont);
    // 设置表格内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    tablesTable->setFont(tableFont);

    int row = 0;
    for (const auto &table : tables) {
        tablesTable->setItem(row, 0, new QTableWidgetItem(QString::number(table.getId())));

        // 如果用户ID为0，显示"无"
        QString userId = table.getUserId() == 0 ? "无" : QString::number(table.getUserId());
        tablesTable->setItem(row, 1, new QTableWidgetItem(userId));

        tablesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(table.getStatus())));

        // 设置每个单元格的居中对齐
        for (int col = 0; col < 3; ++col) {
            tablesTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }

        row++;
    }

    tablesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablesTable->setRowHeight(0, 40);

    // 禁用表格的编辑功能
    tablesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑
}

void viewAllInfoDialog::populateDishesTable()
{
    const auto &allDishes = system_->getDishManager().getDishes();
    std::vector<Dish> dishes;
    for(const auto &a : allDishes){
        if(a.getType() == 1){
            dishes.push_back(a);
        }
    }
    dishesTable->setRowCount(dishes.size());
    dishesTable->setColumnCount(4);
    dishesTable->setHorizontalHeaderLabels({"类型", "ID", "名称", "价格(¥)"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    dishesTable->horizontalHeader()->setFont(headerFont);
    // 设置表格内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    dishesTable->setFont(tableFont);

    int row = 0;
    for (const auto &dish : dishes) {
        if (dish.getType() == 1) {  // 仅显示菜品
            dishesTable->setItem(row, 0, new QTableWidgetItem("菜品"));
            dishesTable->setItem(row, 1, new QTableWidgetItem(QString::number(dish.getId())));
            dishesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dish.getName())));
            dishesTable->setItem(row, 3, new QTableWidgetItem(QString::number(dish.getPrice())));

            // 设置每个单元格的居中对齐
            for (int col = 0; col < 4; ++col) {
                dishesTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
            }

            row++;
        }
    }
    dishesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    dishesTable->setRowHeight(0, 40);

    // 禁用表格的编辑功能
    dishesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑
}

void viewAllInfoDialog::populateDrinksTable()
{
    const auto &allDishes = system_->getDishManager().getDishes();
    std::vector<Dish> dishes;
    for(const auto &a : allDishes){
        if(a.getType() == 2){
            dishes.push_back(a);
        }
    }
    drinksTable->setRowCount(dishes.size());
    drinksTable->setColumnCount(4);
    drinksTable->setHorizontalHeaderLabels({"类型", "ID", "名称", "价格(¥)"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    drinksTable->horizontalHeader()->setFont(headerFont);
    // 设置表格内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    drinksTable->setFont(tableFont);

    int row = 0;
    for (const auto &dish : dishes) {
        if (dish.getType() == 2) {  // 仅显示酒水
            drinksTable->setItem(row, 0, new QTableWidgetItem("酒水"));
            drinksTable->setItem(row, 1, new QTableWidgetItem(QString::number(dish.getId())));
            drinksTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dish.getName())));
            drinksTable->setItem(row, 3, new QTableWidgetItem(QString::number(dish.getPrice())));

            // 设置每个单元格的居中对齐
            for (int col = 0; col < 4; ++col) {
                drinksTable->item(row, col)->setTextAlignment(Qt::AlignCenter);
            }

            row++;
        }
    }
    drinksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    drinksTable->setRowHeight(0, 40);

    // 禁用表格的编辑功能
    drinksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑
}

void viewAllInfoDialog::onUsersButtonClicked()
{
    stackedWidget->setCurrentWidget(usersTable);
}

void viewAllInfoDialog::onTablesButtonClicked()
{
    populateTablesTable();  // 填充餐桌信息表格
    stackedWidget->setCurrentWidget(tablesTable);
}

void viewAllInfoDialog::onDishesButtonClicked()
{
    populateDishesTable();  // 填充菜品信息表格
    stackedWidget->setCurrentWidget(dishesTable);
}

void viewAllInfoDialog::onDrinksButtonClicked()
{
    populateDrinksTable();  // 填充酒水信息表格
    stackedWidget->setCurrentWidget(drinksTable);
}

void viewAllInfoDialog::onCloseButtonClicked()
{
    accept();  // 关闭对话框
}
