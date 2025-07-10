#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>
#include <string>

#include "User.h"

class UserManager {

private:

    std::vector<User> users_;

public:

    // 从文件加载用户数据
    void loadFromFile(const std::string& filename);

    // 保存用户数据到文件
    void saveToFile(const std::string& filename);

    // 验证用户名和密码，若通过将authenticatedUser返回
    bool authenticate(const std::string& username, const std::string& password, User& authenticatedUser);

    // 增加新用户（注册），返回是否添加成功
    bool addUser(const std::string& username, const std::string& password, int role);

    // 按用户名删除用户，返回是否删除成功
    bool removeUser(const std::string& username);

    // 获取指定ID的用户引用，若不存在将抛出异常
    User& getUser(int userId);

    // 获取所有用户列表（只读）
    const std::vector<User>& getUsers() const { return users_; }

};

#endif
