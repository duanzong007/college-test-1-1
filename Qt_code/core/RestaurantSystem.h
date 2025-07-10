#ifndef RESTAURANTSYSTEM_H
#define RESTAURANTSYSTEM_H

#include "UserManager.h"
#include "DishManager.h"
#include "TableManager.h"
#include "OrderManager.h"

class RestaurantSystem {

private:

    UserManager userManager_;
    DishManager dishManager_;
    TableManager tableManager_;
    OrderManager orderManager_;
    User currentUser_;  // 当前登录用户信息

public:

    RestaurantSystem();
    UserManager&        getUserManager()       { return userManager_; }
    const UserManager&  getUserManager() const { return userManager_; }

    DishManager&        getDishManager()       { return dishManager_; }
    const DishManager&  getDishManager() const { return dishManager_; }

    TableManager&       getTableManager()       { return tableManager_; }
    const TableManager& getTableManager() const { return tableManager_; }

    OrderManager&       getOrderManager()       { return orderManager_; }
    const OrderManager& getOrderManager() const { return orderManager_; }

    const User&         getCurrentUser() const { return currentUser_; }
    void                setCurrentUser(const User& user) { currentUser_ = user; }

    // 加载所有数据文件
    void loadAllData();

    // 保存所有数据文件
    void saveAllData();

};

#endif
