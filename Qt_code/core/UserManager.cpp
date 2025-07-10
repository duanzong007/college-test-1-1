#include <fstream>
#include <iostream>
#include <algorithm>

#include "UserManager.h"

using namespace std;

void UserManager::loadFromFile(const std::string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        users_.clear();
        int id, role;
        string username, password;
        while (inFile >> id >> username >> password >> role) {
            users_.push_back(User(id, username, password, role));
        }
        inFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}

void UserManager::saveToFile(const std::string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const User& u : users_) {
            outFile << u.getId() << " "
                    << u.getUsername() << " "
                    << u.getPassword() << " "
                    << u.getRole() << endl;
        }
        outFile.close();
    }
    else {
        cerr << "无法打开文件 " << filename << "！" << endl;
    }
}

bool UserManager::authenticate(const std::string& username, const std::string& password, User& authenticatedUser) {
    for (const User& u : users_) {
        if (u.getUsername() == username && u.getPassword() == password) {
            authenticatedUser = u;
            return true;
        }
    }
    return false;
}

bool UserManager::addUser(const std::string& username, const std::string& password, int role) {
    // 检查用户名是否已存在
    for (const User& u : users_) {
        if (u.getUsername() == username) {
            return false;
        }
    }
    // 寻找最小的未使用用户ID
    int newId = 1;
    bool used;
    do {
        used = false;
        for (const User& u : users_) {
            if (u.getId() == newId) {
                used = true;
                newId++;
                break;
            }
        }
    } while (used);
    // 添加新用户
    users_.push_back(User(newId, username, password, role));
    // 按ID排序用户列表
    sort(users_.begin(), users_.end(), [](const User& a, const User& b) {
        return a.getId() < b.getId();
    });
    return true;
}

bool UserManager::removeUser(const std::string& username) {
    for (auto it = users_.begin(); it != users_.end(); ++it) {
        if (it->getUsername() == username) {
            users_.erase(it);
            return true;
        }
    }
    return false;
}

User& UserManager::getUser(int userId) {
    for (auto& user : users_) {
        if (user.getId() == userId) {
            return user;
        }
    }
    throw std::runtime_error("User not found");
}
