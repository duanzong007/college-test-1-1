#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <vector>
#include <QTimer>

#include "core/TableManager.h"
#include "core/User.h"
#include "getTableDialog.h"

getTableDialog::getTableDialog(RestaurantSystem* system, QWidget* parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("预定餐桌");
    resize(400, 150);

    // 先检查当前用户是否已经预定了桌子
    int userId = system_->getCurrentUser().getId();
    int existingTable = system_->getTableManager().findTableIdByUser(userId);
    if (existingTable != 0)
    {
        // 如果已经预定，直接提示并关闭对话框
        QMessageBox::information(this, "提示", QString("您已预定了桌号 %1").arg(existingTable));
        // 使用单次定时器，延迟 0 毫秒调用 reject()，确保对话框不会显示
        QTimer::singleShot(0, this, SLOT(reject()));
        return;
    }

    setupUI();
    loadFreeTables();
}

getTableDialog::~getTableDialog()
{
}

void getTableDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 设置窗口大小
    resize(500, 200);  // 设置窗口大小为500x200

    QHBoxLayout* selectLayout = new QHBoxLayout;
    QLabel* label = new QLabel("请选择空闲餐桌：", this);

    // 设置正文部分的字体为思源宋体 CN，字号为15，SemiBold 权重
    QFont contentFont("思源宋体 CN", 14); // 设置思源宋体 CN，字号为15
    contentFont.setWeight(QFont::DemiBold); // 设置 SemiBold 权重
    label->setFont(contentFont);

    tableComboBox = new QComboBox(this);
    tableComboBox->setFont(contentFont); // 同样为QComboBox设置思源宋体 CN SemiBold字体
    selectLayout->addWidget(label);
    selectLayout->addWidget(tableComboBox);
    mainLayout->addLayout(selectLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    reserveButton = new QPushButton("预定", this);
    cancelButton = new QPushButton("取消", this);

    // 设置按钮部分的字体为思源宋体 CN，Regular，字号为13
    QFont buttonFont("思源宋体 CN", 11); // 设置思源宋体 CN，字号为13
    buttonFont.setWeight(QFont::Normal); // 设置 Regular 权重
    reserveButton->setFont(buttonFont); // 按钮字体设置为思源宋体 CN Regular
    cancelButton->setFont(buttonFont);  // 按钮字体设置为思源宋体 CN Regular

    // 对齐按钮到右下角
    buttonLayout->addStretch(); // 添加伸缩项，推按钮到右侧
    buttonLayout->addWidget(reserveButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // 连接按钮事件
    connect(reserveButton, &QPushButton::clicked, this, &getTableDialog::onReserve);
    connect(cancelButton, &QPushButton::clicked, this, &getTableDialog::reject);
}


void getTableDialog::loadFreeTables()
{
    tableComboBox->clear();
    const auto& tables = system_->getTableManager().getTables();
    bool hasFree = false;
    // 遍历所有桌子，找到状态为 "空闲" 的桌子
    for (const auto &table : tables)
    {
        if (QString::fromStdString(table.getStatus()) == "空闲")
        {
            hasFree = true;
            QString text = QString("桌号 %1").arg(table.getId());
            tableComboBox->addItem(text, table.getId());
        }
    }
    if (!hasFree)
    {
        QMessageBox::information(this, "提示", "目前没有空闲的餐桌可供预定！");
        reserveButton->setEnabled(false);
    }
}

void getTableDialog::onReserve()
{
    if (tableComboBox->currentIndex() < 0)
    {
        QMessageBox::warning(this, "错误", "请先选择餐桌！");
        return;
    }
    int selectedTableId = tableComboBox->currentData().toInt();

    // 获取当前用户信息
    const User& currentUser = system_->getCurrentUser();
    int userId = currentUser.getId();
    if (userId == 0)
    {
        QMessageBox::warning(this, "错误", "未能获取当前登录用户信息！");
        return;
    }

    // 更新桌子状态：遍历 TableManager 中的 tables，找到匹配的桌子，然后更新状态和用户ID
    auto& tables = const_cast<std::vector<Table>&>(system_->getTableManager().getTables());
    bool found = false;
    for (auto &table : tables)
    {
        if (table.getId() == selectedTableId)
        {
            // 只有空闲桌子才允许预定（理论上这里只显示空闲桌子）
            if (QString::fromStdString(table.getStatus()) == "空闲")
            {
                table.setStatus("已预定");
                table.setUserId(userId);
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        QMessageBox::warning(this, "错误", "选中的餐桌已不可预定！");
        return;
    }

    // 保存数据
    system_->saveAllData();

    QMessageBox::information(this, "预定成功", QString("您已成功预定桌号 %1").arg(selectedTableId));
    accept();
}
