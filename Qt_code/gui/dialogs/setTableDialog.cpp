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

#include "setTableDialog.h"

setTableDialog::setTableDialog(RestaurantSystem *system, QWidget *parent) :
    QDialog(parent), system_(system) {

    // 设置窗口标题
    setWindowTitle("餐位管理");

    // 设置主布局
    mainLayout = new QVBoxLayout(this);

    // 创建顶部的三个按钮，分别对应“添加餐位”、“删除餐位”和“编辑餐位”
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("添加餐位", this);
    removeButton = new QPushButton("删除餐位", this);
    editButton = new QPushButton("编辑餐位", this);

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
    connect(addButton, &QPushButton::clicked, this, &setTableDialog::showAddTable);
    connect(removeButton, &QPushButton::clicked, this, &setTableDialog::showRemoveTable);
    connect(editButton, &QPushButton::clicked, this, &setTableDialog::showEditTable);

    // 初始显示“添加餐位”界面
    showAddTable();

    // 设置窗口尺寸稍微放大
    resize(400, 300);
}

setTableDialog::~setTableDialog() {
    delete mainWidget;
}

void setTableDialog::showAddTable() {
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
    QLabel* label = new QLabel("请输入要添加的餐位数量：", mainWidget);
    label->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    tableCountLineEdit = new QLineEdit(mainWidget);
    confirmAddButton = new QPushButton("确认添加", mainWidget);

    // 设置控件字体
    tableCountLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmAddButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label->setAlignment(Qt::AlignCenter);
    tableCountLineEdit->setAlignment(Qt::AlignCenter);

    // 对齐方式调整，确保输入框不靠下
    newLayout->addWidget(label, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(tableCountLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(confirmAddButton, 0, Qt::AlignCenter);

    connect(confirmAddButton, &QPushButton::clicked, this, &setTableDialog::addTable);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setTableDialog::showRemoveTable() {
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
    QLabel* label = new QLabel("请选择要删除的餐位：", mainWidget);
    label->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    tableListWidget = new QListWidget(mainWidget);
    confirmRemoveButton = new QPushButton("确认删除", mainWidget);

    // 设置控件字体
    tableListWidget->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmRemoveButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label->setAlignment(Qt::AlignCenter);

    // Add items with proper alignment for each item
    for (const Table& table : system_->getTableManager().getTables()) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(std::to_string(table.getId())), tableListWidget);
        item->setTextAlignment(Qt::AlignCenter);  // Set text alignment for each item
    }

    // 对齐方式调整，确保输入框不靠下
    newLayout->addWidget(label, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(tableListWidget, 0, Qt::AlignCenter);
    newLayout->addWidget(confirmRemoveButton, 0, Qt::AlignCenter);

    connect(confirmRemoveButton, &QPushButton::clicked, this, &setTableDialog::removeTable);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setTableDialog::showEditTable() {
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
    QLabel* label1 = new QLabel("选择餐位：", mainWidget);
    QLabel* label2 = new QLabel("使用者ID：", mainWidget);
    QLabel* label3 = new QLabel("餐位状态：", mainWidget);

    label1->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    label2->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    label3->setFont(QFont("思源宋体 CN", 13, QFont::Normal));

    tableSelectComboBox = new QComboBox(mainWidget);
    editUserIdLineEdit = new QLineEdit(mainWidget);
    editStatusComboBox = new QComboBox(mainWidget);
    confirmEditButton = new QPushButton("确认编辑", mainWidget);

    // 设置控件字体
    tableSelectComboBox->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    editUserIdLineEdit->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    editStatusComboBox->setFont(QFont("思源宋体 CN", 13, QFont::Normal));
    confirmEditButton->setFont(QFont("思源宋体 CN", 13, QFont::DemiBold));

    // 使用布局调整控件位置，确保控件居中对齐
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);
    label3->setAlignment(Qt::AlignCenter);

    newLayout->addWidget(label1, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(tableSelectComboBox, 0, Qt::AlignCenter);
    newLayout->addWidget(label2, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(editUserIdLineEdit, 0, Qt::AlignCenter);
    newLayout->addWidget(label3, 0, Qt::AlignTop | Qt::AlignHCenter);
    newLayout->addWidget(editStatusComboBox, 0, Qt::AlignCenter);
    newLayout->addWidget(confirmEditButton, 0, Qt::AlignCenter);

    // 设置餐位状态选项（空闲、使用中、已预定）
    editStatusComboBox->addItem("空闲");
    editStatusComboBox->addItem("使用中");
    editStatusComboBox->addItem("已预定");

    // 加载所有餐位并显示
    for (const Table& table : system_->getTableManager().getTables()) {
        tableSelectComboBox->addItem(QString::fromStdString(std::to_string(table.getId())), QVariant::fromValue(table.getId()));
    }

    // 选择餐位后填充原数据
    connect(tableSelectComboBox, &QComboBox::currentIndexChanged, this, [this]() {
        int selectedTableId = tableSelectComboBox->currentData().toInt();
        if (selectedTableId > 0) {
            Table* table = system_->getTableManager().getTable(selectedTableId);
            editUserIdLineEdit->setText(QString::number(table->getUserId()));
            editStatusComboBox->setCurrentIndex(table->getStatus() == "空闲" ? 0 : (table->getStatus() == "使用中" ? 1 : 2));
        }
    });

    connect(confirmEditButton, &QPushButton::clicked, this, &setTableDialog::editTable);

    mainWidget->setLayout(newLayout);  // 设置新的布局
}

void setTableDialog::removeTable() {
    QList<QListWidgetItem*> selectedItems = tableListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择要删除的餐位！");
        return;
    }

    for (QListWidgetItem* item : selectedItems) {
        int tableId = item->text().toInt();
        system_->getTableManager().removeTable(tableId);
    }

    QMessageBox::information(this, "成功", "餐位删除成功！");
    system_->saveAllData();  // 保存所有数据
    showRemoveTable();  // 重新显示“删除餐位”界面
}

void setTableDialog::editTable() {
    int selectedTableId = tableSelectComboBox->currentData().toInt();  // 获取选中的餐位ID
    if (selectedTableId <= 0) {
        QMessageBox::warning(this, "错误", "请选择有效的餐位！");
        return;
    }

    int newUserId = editUserIdLineEdit->text().toInt();
    QString newStatus = editStatusComboBox->currentText();

    Table* table = system_->getTableManager().getTable(selectedTableId);
    if (table) {
        table->setUserId(newUserId);
        table->setStatus(newStatus.toStdString());  // 修改餐桌状态

        system_->saveAllData();  // 保存所有数据

        QMessageBox::information(this, "成功", "餐位编辑成功！");
        showEditTable();  // 重新显示“编辑餐位”界面
    }
}

void setTableDialog::addTable() {
    int tableCount = tableCountLineEdit->text().toInt();  // 获取要添加的餐位数量

    // 获取现有餐位的ID，找到所有餐位号
    std::vector<int> usedIds;
    const auto& tables = system_->getTableManager().getTables(); // 获取不可修改的餐位列表
    for (const Table& table : tables) {
        usedIds.push_back(table.getId());
    }

    // 排序所有已用餐位ID
    std::sort(usedIds.begin(), usedIds.end());

    // 找到每个空位并分配ID
    int newId = 1;  // 从1开始查找空位
    for (int i = 0; i < tableCount; ++i) {
        // 查找下一个空缺的餐位号
        while (std::find(usedIds.begin(), usedIds.end(), newId) != usedIds.end()) {
            newId++;  // 如果当前ID已被占用，递增，直到找到未占用的ID
        }

        // 添加餐桌，默认餐桌状态为"空闲"，用户ID为0
        bool success = system_->getTableManager().addTable(newId, "空闲");
        if (!success) {
            QMessageBox::warning(this, "失败", "餐位添加失败！");
            return;
        }

        // 添加新餐位后，更新已用ID
        usedIds.push_back(newId); // 把新的ID加入到已用ID列表
    }

    // 添加完餐位后，重新按餐位号排序
    auto& tablesRef = const_cast<std::vector<Table>&>(system_->getTableManager().getTables());
    std::sort(tablesRef.begin(), tablesRef.end(), [](const Table& a, const Table& b) {
        return a.getId() < b.getId();  // 按餐位号升序排序
    });

    QMessageBox::information(this, "成功", "餐位添加成功！");
    system_->saveAllData();  // 保存所有数据
    showAddTable();  // 重新显示“添加餐位”界面
}
