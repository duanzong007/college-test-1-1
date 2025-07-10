#include <fstream>
#include <iostream>
#include <iomanip>

#include "OrderManager.h"

using namespace std;

OrderManager::OrderManager(DishManager* dm, TableManager* tm)
    : dishManager_(dm), tableManager_(tm) {
}

void OrderManager::loadFromFile(const std::string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        orders_.clear();
        int orderId, tableId, itemType, quantity;
        float totalPrice;
        string status,dishname,timeStr;
        while (true) {
            if (!(inFile >> orderId >> tableId >> itemType >> dishname >> quantity >> totalPrice >> status))
                break;
            inFile >> ws;
            getline(inFile, timeStr);
            if (!timeStr.empty() && timeStr[0] == ' ') {
                timeStr = timeStr.substr(1);
            }
            orders_.push_back(Order(orderId, tableId, itemType, dishname, quantity, totalPrice, status, timeStr));
        }
        inFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}

void OrderManager::saveToFile(const std::string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const Order& o : orders_) {
            outFile << o.getOrderId() << " "
                    << o.getTableId() << " "
                    << o.getItemType() << " "
                    << o.getDishname() << " "
                    << o.getQuantity() << " "
                    << fixed << setprecision(2) << o.getTotalPrice() << " "
                    << o.getStatus() << " "
                    << o.getOrderTime() << endl;
        }
        outFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}
