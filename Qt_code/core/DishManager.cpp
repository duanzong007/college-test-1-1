#include <fstream>
#include <iostream>
#include <algorithm>

#include "DishManager.h"

using namespace std;

void DishManager::loadFromFile(const std::string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        dishes_.clear();
        int id, type;
        float price;
        string name;
        // 逐行读取，处理名称中的空格
        while (true) {
            if (!(inFile >> type >> id >> price)) break;
            inFile >> ws;
            string itemName;
            getline(inFile, itemName);
            if (!itemName.empty() && itemName[0] == ' ') {
                itemName = itemName.substr(1);
            }
            dishes_.push_back(Dish(id, itemName, price, type));
        }
        inFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}

void DishManager::saveToFile(const std::string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const Dish& d : dishes_) {
            outFile << d.getType() << " "
                    << d.getId() << " "
                    << d.getPrice() << " "
                    << d.getName() << endl;
        }
        outFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}

bool DishManager::addItem(int id, const std::string& name, float price, int type) {
    // 确认相同类型下ID不重复
    for (const Dish& d : dishes_) {
        if (d.getId() == id && d.getType() == type) {
            return false;
        }
    }
    dishes_.push_back(Dish(id, name, price, type));
    // 按ID排序，方便管理
    sort(dishes_.begin(), dishes_.end(), [](const Dish& a, const Dish& b) {
        return a.getId() < b.getId();
    });
    return true;
}

bool DishManager::removeItem(int id, int type) {
    for (auto it = dishes_.begin(); it != dishes_.end(); ++it) {
        if (it->getId() == id && it->getType() == type) {
            dishes_.erase(it);
            return true;
        }
    }
    return false;
}

Dish& DishManager::getDish(int dishId, int type) {
    for (Dish& dish : dishes_) {
        // 只返回类型匹配的菜品
        if (dish.getId() == dishId && dish.getType() == type) {
            return dish;
        }
    }
    throw std::runtime_error("Dish not found or type mismatch");
}


