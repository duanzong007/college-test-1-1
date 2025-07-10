#ifndef TABLE_H
#define TABLE_H

#include <string>

class Table {

private:

    int id_;
    int userId_;        // 当前占用/预定此餐桌的用户ID（空闲则为0）
    std::string status_; // 餐桌状态: "空闲", "使用中", "已预定"

public:

    Table() : id_(0), userId_(0), status_("空闲") {}
    Table(int id, int userId, const std::string& status)
        : id_(id), userId_(userId), status_(status) {
    }

    // Getter方法
    int getId() const { return id_; }
    int getUserId() const { return userId_; }
    std::string getStatus() const { return status_; }

    // Setter方法
    void setUserId(int uid) { userId_ = uid; }
    void setStatus(const std::string& status) { status_ = status; }
};

#endif
