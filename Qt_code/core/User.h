#ifndef USER_H
#define USER_H

#include <string>
#include <QMetaType>

class User {

private:

    int id_;
    std::string username_;
    std::string password_;
    int role_; // 1: 管理员, 2: 服务员, 3: 厨师, 4: 收银员, 5: 老板, 6: 消费者

public:

    User() : id_(0), role_(0) {}
    User(int id, const std::string& username, const std::string& password, int role)
        : id_(id), username_(username), password_(password), role_(role) {
    }

    // Getter方法
    int getId() const { return id_; }
    std::string getUsername() const { return username_; }
    std::string getPassword() const { return password_; }
    int getRole() const { return role_; }

    // Setter方法
    void setPassword(const std::string& newPassword) { password_ = newPassword; }

};

Q_DECLARE_METATYPE(User);
#endif
