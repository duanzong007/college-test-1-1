#include <QDebug>

#include "RestaurantSystem.h"

using namespace std;

RestaurantSystem::RestaurantSystem()
    : orderManager_(&dishManager_, &tableManager_) {
    // 默认构造，各管理器已就绪
}

void RestaurantSystem::loadAllData() {
    userManager_.loadFromFile("users.txt");
    dishManager_.loadFromFile("dishes.txt");
    tableManager_.loadFromFile("tables.txt");
    orderManager_.loadFromFile("orders.txt");
}

void RestaurantSystem::saveAllData() {
    userManager_.saveToFile("users.txt");
    dishManager_.saveToFile("dishes.txt");
    tableManager_.saveToFile("tables.txt");
    orderManager_.saveToFile("orders.txt");
    qDebug() << "所有数据已保存。\n";
}
