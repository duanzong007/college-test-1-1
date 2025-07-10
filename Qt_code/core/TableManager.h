#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include <vector>
#include <string>

#include "Table.h"

class TableManager {

private:

    std::vector<Table> tables_;

public:

    // 从文件加载餐桌数据
    void loadFromFile(const std::string& filename);

    // 保存餐桌数据到文件
    void saveToFile(const std::string& filename);

    // 将已预定餐桌状态改为使用中（在首次点餐时调用）
    void useTable(int tableId);

    // 根据用户ID查找其占用/预定的餐桌ID，找不到返回0
    int findTableIdByUser(int userId) const;

    // 获取指定餐桌
    Table* getTable(int id);

    // 添加餐桌
    bool addTable(int id, const std::string& status);

    // 删除餐桌
    bool removeTable(int id);

    // 获取所有餐桌的列表
    const std::vector<Table>& getTables() const { return tables_; }

};

#endif
