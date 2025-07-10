#ifndef SETDRINKDIALOG_H
#define SETDRINKDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "core/RestaurantSystem.h"

class setDrinkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setDrinkDialog(RestaurantSystem *system, QWidget *parent = nullptr);
    ~setDrinkDialog();

private slots:
    void showAddDrink();   // 显示添加酒水界面
    void showRemoveDrink(); // 显示删除酒水界面
    void showEditDrink();  // 显示编辑酒水界面

    void addDrink();    // 添加酒水
    void removeDrink(); // 删除酒水
    void editDrink();   // 编辑酒水

private:
    RestaurantSystem* system_;  // 后端系统接口

    // 控件声明
    QPushButton* addButton;     // 增加酒水按钮
    QPushButton* removeButton;  // 删除酒水按钮
    QPushButton* editButton;    // 编辑酒水按钮

    QWidget* mainWidget;        // 主界面显示区域
    QVBoxLayout* mainLayout;     // 主布局

    // 添加酒水的控件
    QLineEdit* nameLineEdit;    // 酒水名称输入框
    QLineEdit* priceLineEdit;   // 酒水价格输入框
    QPushButton* confirmAddButton;  // 确认添加按钮

    // 删除酒水的控件
    QListWidget* drinkListWidget;   // 显示酒水的列表框
    QPushButton* confirmRemoveButton; // 确认删除按钮

    // 编辑酒水的控件
    QComboBox* drinkSelectComboBox;   // 酒水选择框
    QLineEdit* editNameLineEdit;     // 编辑酒水名称输入框
    QLineEdit* editPriceLineEdit;    // 编辑酒水价格输入框
    QPushButton* confirmEditButton;  // 确认编辑按钮
};

#endif // SETDRINKDIALOG_H
