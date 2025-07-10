#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QFont>
#include "timeOrderDialog.h"
#include "core/RestaurantSystem.h"

timeOrderDialog::timeOrderDialog(RestaurantSystem *system, QWidget *parent)
    : QDialog(parent), system_(system)
{
    setWindowTitle("消费时段分布");
    resize(600, 400);

    // 创建布局和表格
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建表格控件
    QTableWidget *tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(2);  // 时段列和消费总金额列
    tableWidget->setHorizontalHeaderLabels({"时段", "消费总金额"});

    // 设置表头字体
    QFont headerFont("思源宋体 CN", 13, QFont::DemiBold);
    tableWidget->horizontalHeader()->setFont(headerFont);

    // 设置表格内容字体
    QFont tableFont("思源宋体 CN", 13, QFont::Normal);
    tableWidget->setFont(tableFont);

    // 禁用表格的编辑功能
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁用编辑

    // 填充表格数据
    std::map<int, float> timeDist;  // key: 小时, value: 该小时的消费总金额
    const auto &orders = system_->getOrderManager().getOrders();
    for (const auto &order : orders) {
        std::string timeStr = order.getOrderTime();  // 格式 "YYYY-MM-DD HH:MM:SS"
        try {
            int hour = std::stoi(timeStr.substr(11, 2));  // 提取小时
            timeDist[hour] += order.getTotalPrice();  // 累计该时段的消费金额
        }
        catch (std::exception &e) {
            // 解析失败，忽略该订单的时间
        }
    }

    // 将map数据按小时顺序填充到表格中
    tableWidget->setRowCount(timeDist.size());
    int row = 0;
    for (const auto &entry : timeDist) {
        int hour = entry.first;
        float total = entry.second;

        // 格式化时段字符串为 "HH:00 - HH+1:00"，确保小时是两位数字
        QString timeInterval = QString("%1:00 - %2:00")
                                   .arg(hour, 2, 10, QChar('0'))
                                   .arg(hour + 1, 2, 10, QChar('0'));
        tableWidget->setItem(row, 0, new QTableWidgetItem(timeInterval));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(total, 'f', 2)));

        // 设置每个单元格的居中对齐
        for (int col = 0; col < 2; ++col) {
            tableWidget->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }

        row++;
    }

    // 设置表格列宽自动调整
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 将表格添加到布局中
    mainLayout->addWidget(tableWidget);

    // 创建关闭按钮
    QPushButton *closeButton = new QPushButton("关闭", this);
    closeButton->setFont(QFont("思源宋体 CN", 11, QFont::Normal));  // 设置按钮字体
    connect(closeButton, &QPushButton::clicked, this, &timeOrderDialog::close);

    // 将关闭按钮放在右下角
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

timeOrderDialog::~timeOrderDialog()
{
    // 无需特殊资源释放
}
