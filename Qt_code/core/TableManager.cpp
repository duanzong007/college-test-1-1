#include <fstream>
#include <iostream>
#include <algorithm>

#include "TableManager.h"

using namespace std;

void TableManager::loadFromFile(const std::string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        tables_.clear();
        int id, userId;
        string status;
        while (true) {
            if (!(inFile >> id >> userId)) break;
            inFile >> ws;
            string statLine;
            getline(inFile, statLine);
            if (!statLine.empty() && statLine[0] == ' ') {
                statLine = statLine.substr(1);
            }
            status = statLine;
            tables_.push_back(Table(id, userId, status));
        }
        inFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}

void TableManager::saveToFile(const std::string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const Table& t : tables_) {
            outFile << t.getId() << " "
                    << t.getUserId() << " "
                    << t.getStatus() << endl;
        }
        outFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}

void TableManager::useTable(int tableId) {
    for (Table& t : tables_) {
        if (t.getId() == tableId) {
            if (t.getStatus() == "已预定") {
                t.setStatus("使用中");
                cout << "餐桌 " << tableId << " 状态已更新为 使用中\n";
            }
            return;
        }
    }
}

int TableManager::findTableIdByUser(int userId) const {
    for (const Table& t : tables_) {
        if (t.getUserId() == userId) {
            return t.getId();
        }
    }
    return 0;
}

Table* TableManager::getTable(int id) {
    for (auto& table : tables_) {
        if (table.getId() == id) {
            return &table;
        }
    }
    return nullptr; // 如果找不到餐桌返回nullptr
}

bool TableManager::addTable(int id, const std::string& status) {
    tables_.emplace_back(id, 0, status); // 默认餐桌空闲且无用户
    return true;
}

bool TableManager::removeTable(int id) {
    auto it = std::remove_if(tables_.begin(), tables_.end(),
                             [id](const Table& table) { return table.getId() == id; });

    if (it != tables_.end()) {
        tables_.erase(it, tables_.end());
        return true;
    }
    return false; // 如果没有找到餐桌ID，返回false
}

