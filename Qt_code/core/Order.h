#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order {

private:

    int orderId_;
    int tableId_;
    int itemType_;    // 1: 菜品订单, 2: 酒水订单
    std::string dishname;
    int quantity_;
    float totalPrice_;
    std::string status_;    // 订单状态
    std::string orderTime_; // 下单时间，格式 "YYYY-MM-DD HH:MM:SS"

public:

    Order() : orderId_(0), tableId_(0), itemType_(0), dishname(""), quantity_(0), totalPrice_(0.0f), status_(""), orderTime_("") {}
    Order(int orderId, int tableId, int itemType, std::string &dishname_, int quantity, float totalPrice,
          const std::string& status, const std::string& orderTime)
        : orderId_(orderId), tableId_(tableId), itemType_(itemType), dishname(dishname_),
        quantity_(quantity), totalPrice_(totalPrice), status_(status), orderTime_(orderTime) {
    }

    // Getter方法
    int getOrderId() const { return orderId_; }
    int getTableId() const { return tableId_; }
    int getItemType() const { return itemType_; }
    std::string getDishname() const { return dishname; }
    int getQuantity() const { return quantity_; }
    float getTotalPrice() const { return totalPrice_; }
    std::string getStatus() const { return status_; }
    std::string getOrderTime() const { return orderTime_; }

    // Setter方法
    void setStatus(const std::string& newStatus) { status_ = newStatus; }
    void setTotalPrice(const float& newTotalPrice) { totalPrice_ = newTotalPrice; }


};

#endif
