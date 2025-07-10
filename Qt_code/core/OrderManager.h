#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <vector>
#include <string>

#include "Order.h"
#include "DishManager.h"
#include "TableManager.h"

class OrderManager {

private:

    std::vector<Order> orders_;
    DishManager* dishManager_;   // 指向菜品管理，用于查询菜名和价格
    TableManager* tableManager_; // 指向餐桌管理，用于更新餐桌状态

public:

    // 从文件加载订单数据
    void loadFromFile(const std::string& filename);

    // 保存订单数据到文件
    void saveToFile(const std::string& filename);

    // 构造时传入DishManager和TableManager指针
    OrderManager(DishManager* dm, TableManager* tm);

    // 获取所有订单列表（常量引用）
    const std::vector<Order>& getOrders() const { return orders_; }


};

#endif
