#ifndef DISH_H
#define DISH_H

#include <string>

class Dish {

private:

    int id_;
    std::string name_;
    float price_;
    int type_; // 1: 菜品, 2: 酒水

public:

    Dish() : id_(0), price_(0.0f), type_(0) {}
    Dish(int id, const std::string& name, float price, int type)
        : id_(id), name_(name), price_(price), type_(type) {
    }

    // Getter方法
    int getId() const { return id_; }
    std::string getName() const { return name_; }
    float getPrice() const { return price_; }
    int getType() const { return type_; }

    // Setter方法
    void setName(const std::string& newName) { name_ = newName; }
    void setPrice(float newPrice) { price_ = newPrice; }

};

#endif
