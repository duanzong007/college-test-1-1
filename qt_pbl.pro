QT += core gui
QT += quick qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
DEFINES += _CRT_SECURE_NO_WARNINGS


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/DishManager.cpp \
    core/OrderManager.cpp \
    core/RestaurantSystem.cpp \
    core/TableManager.cpp \
    core/UserManager.cpp \
    gui/dialogs/CaddOrderDialog.cpp \
    gui/dialogs/CviewOrderDialog.cpp \
    gui/dialogs/addOrderDialog.cpp \
    gui/dialogs/addUserDialog.cpp \
    gui/dialogs/checkOrderDialog.cpp \
    gui/dialogs/dailyDishSummaryDialog.cpp \
    gui/dialogs/dailyOrderSummaryDialog.cpp \
    gui/dialogs/deleteUserDialog.cpp \
    gui/dialogs/dishRankingDialog.cpp \
    gui/dialogs/editDishStatusDialog.cpp \
    gui/dialogs/getTableDialog.cpp \
    gui/dialogs/passwordDialog.cpp \
    gui/dialogs/setDishDialog.cpp \
    gui/dialogs/setDrinkDialog.cpp \
    gui/dialogs/setTableDialog.cpp \
    gui/dialogs/timeOrderDialog.cpp \
    gui/dialogs/viewAllInfoDialog.cpp \
    gui/dialogs/viewAllOrderDialog.cpp \
    gui/dialogs/viewOrderDialog.cpp \
    gui/dialogs/viewPendingDialog.cpp \
    gui/logindialog.cpp \
    gui/mainwindow.cpp \
    gui/pages/adminpage.cpp \
    gui/pages/bosspage.cpp \
    gui/pages/cashierpage.cpp \
    gui/pages/chefpage.cpp \
    gui/pages/consumerpage.cpp \
    gui/pages/waiterpage.cpp \
    main.cpp
HEADERS += \
    core/Dish.h \
    core/DishManager.h \
    core/Order.h \
    core/OrderManager.h \
    core/RestaurantSystem.h \
    core/Table.h \
    core/TableManager.h \
    core/User.h \
    core/UserManager.h \
    gui/dialogs/CaddOrderDialog.h \
    gui/dialogs/CviewOrderDialog.h \
    gui/dialogs/addOrderDialog.h \
    gui/dialogs/addUserDialog.h \
    gui/dialogs/checkOrderDialog.h \
    gui/dialogs/dailyDishSummaryDialog.h \
    gui/dialogs/dailyOrderSummaryDialog.h \
    gui/dialogs/deleteUserDialog.h \
    gui/dialogs/dishRankingDialog.h \
    gui/dialogs/editDishStatusDialog.h \
    gui/dialogs/getTableDialog.h \
    gui/dialogs/passwordDialog.h \
    gui/dialogs/setDishDialog.h \
    gui/dialogs/setDrinkDialog.h \
    gui/dialogs/setTableDialog.h \
    gui/dialogs/timeOrderDialog.h \
    gui/dialogs/viewAllInfoDialog.h \
    gui/dialogs/viewAllOrderDialog.h \
    gui/dialogs/viewOrderDialog.h \
    gui/dialogs/viewPendingDialog.h \
    gui/logindialog.h \
    gui/mainwindow.h \
    gui/pages/adminpage.h \
    gui/pages/bosspage.h \
    gui/pages/cashierpage.h \
    gui/pages/chefpage.h \
    gui/pages/consumerpage.h \
    gui/pages/waiterpage.h

FORMS += \
    gui/dialogs/CaddOrderDialog.ui \
    gui/dialogs/CviewOrderDialog.ui \
    gui/dialogs/addOrderDialog.ui \
    gui/dialogs/addUserDialog.ui \
    gui/dialogs/checkOrderDialog.ui \
    gui/dialogs/dailyDishSummaryDialog.ui \
    gui/dialogs/dailyOrderSummaryDialog.ui \
    gui/dialogs/deleteUserDialog.ui \
    gui/dialogs/dishRankingDialog.ui \
    gui/dialogs/editDishStatusDialog.ui \
    gui/dialogs/getTableDialog.ui \
    gui/dialogs/passwordDialog.ui \
    gui/dialogs/setDishDialog.ui \
    gui/dialogs/setDrinkDialog.ui \
    gui/dialogs/setTableDialog.ui \
    gui/dialogs/timeOrderDialog.ui \
    gui/dialogs/viewAllInfoDialog.ui \
    gui/dialogs/viewAllOrderDialog.ui \
    gui/dialogs/viewOrderDialog.ui \
    gui/dialogs/viewPendingDialog.ui \
    gui/logindialog.ui \
    gui/mainwindow.ui \
    gui/pages/adminpage.ui \
    gui/pages/bosspage.ui \
    gui/pages/cashierpage.ui \
    gui/pages/chefpage.ui \
    gui/pages/consumerpage.ui \
    gui/pages/waiterpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/png.qrc
