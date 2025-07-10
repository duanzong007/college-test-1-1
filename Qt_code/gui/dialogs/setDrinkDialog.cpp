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

#include "setDrinkDialog.h"

setDrinkDialog::setDrinkDialog(RestaurantSystem *system, QWidget *parent) :
    QDialog(parent), system_(system) {

    // 设置窗口标题
    setWindowTitle("酒水管理");

    // 设置主布局
    mainLayout = new QVBoxLayout(this);

    // 创建顶部的三个按钮，分别对应“添加酒水”、“删除酒水”和“编辑酒水”
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("添加酒水", this);
    removeButton = new QPushButton("删除酒水", this);
    editButton = new QPushButton("编辑酒水", this);

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
    connect(addButton, &QPushButton::clicked, this, &setDrinkDialog::showAddDrink);
    connect(removeButton, &QPushButton::clicked, this, &setDrinkDialog::showRemoveDrink);
    connect(editButton, &QPushButton::clicked, this, &setDrinkDialog::showEditDrink);

    // 初始显示“添加酒水”界面
    showAddDrink();

    // 设置窗口尺寸稍微放大
    resize(400, 300);
}

setDrinkDialog::~setDrinkDialog() {
    delete mainWidget;
}

void setDrinkDialog::showAddDrink() {
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
    QLabel* label = new QLabel("请输入要添加的酒水名称和价格：", mainWidget);
    label->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    nameLineEdit = new QLineEdit(mainWidget);
    nameLineEdit->setPlaceholderText("酒水名称");  // 提示文本

    priceLineEdit = new QLineEdit(mainWidget);
    priceLineEdit->setPlaceholderText("酒水价格");  // 提示文本

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

    connect(confirmAddButton, &QPushButton::clicked, this, &setDrinkDialog::addDrink);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setDrinkDialog::addDrink() {
    QString name = nameLineEdit->text();
    float price = priceLineEdit->text().toFloat();

    // 获取最小可用的ID，确保ID不与现有酒水ID重复
    int newId = 1;

    std::vector<int> existingIds;
    for (const Dish& drink : system_->getDishManager().getDishes()) {
        if (drink.getType() == 2) {  // 只考虑酒水
            existingIds.push_back(drink.getId());
        }
    }

    // 确保新ID不重复，找到最小未使用的ID
    while (std::find(existingIds.begin(), existingIds.end(), newId) != existingIds.end()) {
        ++newId;
    }

    bool success = system_->getDishManager().addItem(newId, name.toStdString(), price, 2); // 2表示酒水
    if (success) {
        QMessageBox::information(this, "成功", "酒水添加成功！");
        system_->saveAllData();  // 保存所有数据
        showAddDrink();  // 重新显示“添加酒水”界面
    } else {
        QMessageBox::warning(this, "失败", "酒水添加失败！");
    }
}

void setDrinkDialog::showRemoveDrink() {
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
    drinkListWidget = new QListWidget(mainWidget);
    confirmRemoveButton = new QPushButton("确认删除", mainWidget);

    // 标签字体设置
    QLabel* label = new QLabel("请选择要删除的酒水：", mainWidget);
    label->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    newLayout->addWidget(label);
    newLayout->addWidget(drinkListWidget);
    newLayout->addWidget(confirmRemoveButton);

    // 设置控件字体
    drinkListWidget->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmRemoveButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label->setAlignment(Qt::AlignCenter);

    // Add items with proper alignment for each item
    for (const Dish& drink : system_->getDishManager().getDishes()) {
        if (drink.getType() == 2) {  // 只显示酒水
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(drink.getName()), drinkListWidget);
            item->setTextAlignment(Qt::AlignCenter);  // Set text alignment for each item
        }
    }

    connect(confirmRemoveButton, &QPushButton::clicked, this, &setDrinkDialog::removeDrink);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setDrinkDialog::removeDrink() {
    QList<QListWidgetItem*> selectedItems = drinkListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择要删除的酒水！");
        return;
    }

    for (QListWidgetItem* item : selectedItems) {
        QString drinkName = item->text();
        int id = -1;
        for (const Dish& drink : system_->getDishManager().getDishes()) {
            if (drink.getName() == drinkName.toStdString() && drink.getType() == 2) {  // 只删除酒水
                id = drink.getId();
                break;
            }
        }
        if (id != -1) {
            system_->getDishManager().removeItem(id, 2);
        }
    }

    QMessageBox::information(this, "成功", "酒水删除成功！");
    system_->saveAllData();  // 保存所有数据
    showRemoveDrink();  // 重新显示“删除酒水”界面
}

void setDrinkDialog::showEditDrink() {
    // 清空当前界面
    QLayout* layout = mainWidget->layout();
    if (layout) {
        // 先移除布局内的所有控件
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;  // 删除布局
    }

    // 创建新的布局
    QVBoxLayout* newLayout = new QVBoxLayout(mainWidget);

    // 标签字体设置
    QLabel* label1 = new QLabel("选择酒水：", mainWidget);
    QLabel* label2 = new QLabel("酒水名称：", mainWidget);
    QLabel* label3 = new QLabel("酒水价格：", mainWidget);

    label1->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    label2->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    label3->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    // 设置控件
    drinkSelectComboBox = new QComboBox(mainWidget);
    editNameLineEdit = new QLineEdit(mainWidget);
    editPriceLineEdit = new QLineEdit(mainWidget);
    confirmEditButton = new QPushButton("确认编辑", mainWidget);

    drinkSelectComboBox->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    editNameLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    editPriceLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmEditButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 对齐方式调整，确保控件居中
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);
    label3->setAlignment(Qt::AlignCenter);

    // 设置控件位置和对齐
    newLayout->addWidget(label1, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(drinkSelectComboBox, 0, Qt::AlignCenter);
    newLayout->addWidget(label2, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(editNameLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(label3, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(editPriceLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(confirmEditButton, 0, Qt::AlignCenter);

    // 在 QComboBox 中添加“待选”项作为默认项
    drinkSelectComboBox->addItem("待选", QVariant());  // 添加一个默认的“待选”项，存储为空数据

    // 加载所有酒水（type == 2）并显示
    for (const Dish& drink : system_->getDishManager().getDishes()) {
        if (drink.getType() == 2) {  // 只加载酒水
            drinkSelectComboBox->addItem(QString::fromStdString(drink.getName()), QVariant::fromValue(drink.getId()));
        }
    }

    // 设置默认选择项为“待选”
    drinkSelectComboBox->setCurrentIndex(0);

    // 选择酒水后填充原数据
    connect(drinkSelectComboBox, &QComboBox::currentIndexChanged, this, [this]() {
        int selectedDrinkId = drinkSelectComboBox->currentData().toInt();  // 获取选中的酒水ID
        if (selectedDrinkId > 0) {
            // 获取对应的酒水，类型为2
            Dish& drink = system_->getDishManager().getDish(selectedDrinkId, 2);  // 获取类型为酒水的酒水
            editNameLineEdit->setText(QString::fromStdString(drink.getName()));
            editPriceLineEdit->setText(QString::number(drink.getPrice()));
        } else {
            // 如果选择的是“待选”，清空输入框
            editNameLineEdit->clear();
            editPriceLineEdit->clear();
        }
    });

    connect(confirmEditButton, &QPushButton::clicked, this, &setDrinkDialog::editDrink);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setDrinkDialog::editDrink() {
    int selectedDrinkId = drinkSelectComboBox->currentData().toInt();  // 获取选中的酒水ID
    if (selectedDrinkId <= 0) {
        QMessageBox::warning(this, "错误", "请选择有效的酒水！");
        return;
    }

    // 获取新的酒水名称和价格
    QString newName = editNameLineEdit->text();
    float newPrice = editPriceLineEdit->text().toFloat();

    // 获取酒水对象并更新数据
    Dish& drink = system_->getDishManager().getDish(selectedDrinkId, 2);  // 获取类型为酒水的酒水
    drink.setName(newName.toStdString());
    drink.setPrice(newPrice);

    // 保存修改后的数据
    system_->saveAllData();  // 保存所有数据

    QMessageBox::information(this, "成功", "酒水编辑成功！");
    showEditDrink();  // 重新显示“编辑酒水”界面
}
