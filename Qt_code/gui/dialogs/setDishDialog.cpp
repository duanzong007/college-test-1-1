#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QListWidgetItem>

#include "setDishDialog.h"

setDishDialog::setDishDialog(RestaurantSystem *system, QWidget *parent) :
    QDialog(parent), system_(system) {

    // 设置窗口标题
    setWindowTitle("菜品管理");

    // 设置主布局
    mainLayout = new QVBoxLayout(this);

    // 创建顶部的三个按钮，分别对应“添加菜品”、“删除菜品”和“编辑菜品”
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("添加菜品", this);
    removeButton = new QPushButton("删除菜品", this);
    editButton = new QPushButton("编辑菜品", this);

    // 为每个按钮设置字体
    addButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));
    removeButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));
    editButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(editButton);
    mainLayout->addLayout(buttonLayout);

    // 初始化主窗口显示区
    mainWidget = new QWidget(this);
    mainLayout->addWidget(mainWidget);

    // 连接按钮与槽函数
    connect(addButton, &QPushButton::clicked, this, &setDishDialog::showAddDish);
    connect(removeButton, &QPushButton::clicked, this, &setDishDialog::showRemoveDish);
    connect(editButton, &QPushButton::clicked, this, &setDishDialog::showEditDish);

    // 初始显示“添加菜品”界面
    showAddDish();

    // 设置窗口尺寸稍微放大
    resize(400, 300);
}

setDishDialog::~setDishDialog() {
    delete mainWidget;
}

void setDishDialog::showAddDish() {
    // 清空当前界面
    QLayout* layout = mainWidget->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }

    // 创建新的布局
    QVBoxLayout* newLayout = new QVBoxLayout(mainWidget);

    // 标签字体设置
    QLabel* label = new QLabel("请输入要添加的菜品名称和价格：", mainWidget);
    label->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    nameLineEdit = new QLineEdit(mainWidget);
    nameLineEdit->setPlaceholderText("菜品名称");  // 提示文本

    priceLineEdit = new QLineEdit(mainWidget);
    priceLineEdit->setPlaceholderText("菜品价格");  // 提示文本

    confirmAddButton = new QPushButton("确认添加", mainWidget);

    // 设置控件字体
    nameLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    priceLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmAddButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label->setAlignment(Qt::AlignCenter);
    nameLineEdit->setAlignment(Qt::AlignCenter);
    priceLineEdit->setAlignment(Qt::AlignCenter);

    // 对齐方式调整，确保输入框不靠下
    newLayout->addWidget(label, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(nameLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(priceLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(confirmAddButton, 0, Qt::AlignCenter);

    connect(confirmAddButton, &QPushButton::clicked, this, &setDishDialog::addDish);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setDishDialog::addDish() {
    QString name = nameLineEdit->text();
    float price = priceLineEdit->text().toFloat();

    // 获取最小可用的ID，确保ID不与现有菜品ID重复
    int newId = 1;
    std::vector<int> existingIds;
    for (const Dish& dish : system_->getDishManager().getDishes()) {
        if (dish.getType() == 1) {  // 只考虑菜品
            existingIds.push_back(dish.getId());
        }
    }

    // 确保新ID不重复，找到最小未使用的ID
    while (std::find(existingIds.begin(), existingIds.end(), newId) != existingIds.end()) {
        ++newId;
    }

    bool success = system_->getDishManager().addItem(newId, name.toStdString(), price, 1);  // 1表示菜品
    if (success) {
        QMessageBox::information(this, "成功", "菜品添加成功！");
        system_->saveAllData();  // 保存所有数据
        showAddDish();  // 重新显示“添加菜品”界面
    } else {
        QMessageBox::warning(this, "失败", "菜品添加失败！");
    }
}

void setDishDialog::showRemoveDish() {
    // 清空当前界面
    QLayout* layout = mainWidget->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }

    // 创建新的布局
    QVBoxLayout* newLayout = new QVBoxLayout(mainWidget);

    // 标签字体设置
    QLabel* label = new QLabel("请选择要删除的菜品：", mainWidget);
    label->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    dishListWidget = new QListWidget(mainWidget);
    confirmRemoveButton = new QPushButton("确认删除", mainWidget);

    // 设置控件字体
    dishListWidget->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmRemoveButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label->setAlignment(Qt::AlignCenter);

    // Add items with proper alignment for each item
    for (const Dish& dish : system_->getDishManager().getDishes()) {
        if (dish.getType() == 1) {  // 只显示菜品
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(dish.getName()), dishListWidget);
            item->setTextAlignment(Qt::AlignCenter);  // Set text alignment for each item
        }
    }

    newLayout->addWidget(label, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(dishListWidget, 0, Qt::AlignCenter);
    newLayout->addWidget(confirmRemoveButton, 0, Qt::AlignCenter);

    connect(confirmRemoveButton, &QPushButton::clicked, this, &setDishDialog::removeDish);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setDishDialog::removeDish() {
    QList<QListWidgetItem*> selectedItems = dishListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择要删除的菜品！");
        return;
    }

    for (QListWidgetItem* item : selectedItems) {
        QString dishName = item->text();
        int id = -1;
        for (const Dish& dish : system_->getDishManager().getDishes()) {
            if (dish.getName() == dishName.toStdString() && dish.getType() == 1) {
                id = dish.getId();
                break;
            }
        }
        if (id != -1) {
            system_->getDishManager().removeItem(id, 1);
        }
    }

    QMessageBox::information(this, "成功", "菜品删除成功！");
    system_->saveAllData();
    showRemoveDish();  // 重新显示“删除菜品”界面
}

void setDishDialog::showEditDish() {
    // 清空当前界面
    QLayout* layout = mainWidget->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }

    // 创建新的布局
    QVBoxLayout* newLayout = new QVBoxLayout(mainWidget);

    // 标签字体设置
    QLabel* label1 = new QLabel("选择菜品：", mainWidget);
    QLabel* label2 = new QLabel("菜品名称：", mainWidget);
    QLabel* label3 = new QLabel("菜品价格：", mainWidget);

    label1->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    label2->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    label3->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    dishSelectComboBox = new QComboBox(mainWidget);
    editNameLineEdit = new QLineEdit(mainWidget);
    editPriceLineEdit = new QLineEdit(mainWidget);
    confirmEditButton = new QPushButton("确认编辑", mainWidget);

    dishSelectComboBox->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    editNameLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    editPriceLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmEditButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);
    label3->setAlignment(Qt::AlignCenter);

    newLayout->addWidget(label1, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(dishSelectComboBox, 0, Qt::AlignCenter);
    newLayout->addWidget(label2, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(editNameLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(label3, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(editPriceLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(confirmEditButton, 0, Qt::AlignCenter);

    // 添加菜品选择
    dishSelectComboBox->addItem("请选择", QVariant());

    for (const Dish& dish : system_->getDishManager().getDishes()) {
        if (dish.getType() == 1) {
            dishSelectComboBox->addItem(QString::fromStdString(dish.getName()), QVariant::fromValue(dish.getId()));
        }
    }

    dishSelectComboBox->setCurrentIndex(0);

    connect(dishSelectComboBox, &QComboBox::currentIndexChanged, this, [this]() {
        int selectedDishId = dishSelectComboBox->currentData().toInt();
        if (selectedDishId > 0) {
            Dish& dish = system_->getDishManager().getDish(selectedDishId, 1);
            editNameLineEdit->setText(QString::fromStdString(dish.getName()));
            editPriceLineEdit->setText(QString::number(dish.getPrice()));
        } else {
            editNameLineEdit->clear();
            editPriceLineEdit->clear();
        }
    });

    connect(confirmEditButton, &QPushButton::clicked, this, &setDishDialog::editDish);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setDishDialog::editDish() {
    int selectedDishId = dishSelectComboBox->currentData().toInt();
    if (selectedDishId <= 0) {
        QMessageBox::warning(this, "错误", "请选择有效的菜品！");
        return;
    }

    QString newName = editNameLineEdit->text();
    float newPrice = editPriceLineEdit->text().toFloat();

    Dish& dish = system_->getDishManager().getDish(selectedDishId, 1);
    dish.setName(newName.toStdString());
    dish.setPrice(newPrice);

    system_->saveAllData();

    QMessageBox::information(this, "成功", "菜品编辑成功！");
    showEditDish();
}
