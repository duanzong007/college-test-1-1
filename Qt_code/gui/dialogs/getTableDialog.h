#ifndef GETTABLEDIALOG_H
#define GETTABLEDIALOG_H

#include <QDialog>

#include "core/RestaurantSystem.h"

class QComboBox;
class QPushButton;

class getTableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit getTableDialog(RestaurantSystem* system, QWidget* parent = nullptr);
    ~getTableDialog();

private slots:
    void onReserve();

private:
    void setupUI();
    void loadFreeTables();

    RestaurantSystem* system_;
    QComboBox* tableComboBox;  // 显示所有空闲桌子
    QPushButton* reserveButton;
    QPushButton* cancelButton;
};

#endif // GETTABLEDIALOG_H
