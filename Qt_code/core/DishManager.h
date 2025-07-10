#ifndef DISHMANAGER_H
#define DISHMANAGER_H

#include <vector>
#include <string>
#include "Dish.h"

class DishManager {

private:

    std::vector<Dish> dishes_;

public:

    // 从文件加载菜品和酒水数据
    void loadFromFile(const std::string& filename);

    // 保存菜品和酒水数据到文件
    void saveToFile(const std::string& filename);

    // 添加新的菜品或酒水，指定类型（1菜品，2酒水），返回是否添加成功
    bool addItem(int id, const std::string& name, float price, int type);

    // 删除指定ID和类型的菜品/酒水，返回是否删除成功
    bool removeItem(int id, int type);

    //获取指定ID的菜品
    Dish& getDish(int dishId, int type);

    // 获取所有菜品和酒水
    const std::vector<Dish>& getDishes() const { return dishes_; }
          std::vector<Dish>& getDishes() { return dishes_; }

};

#endif
