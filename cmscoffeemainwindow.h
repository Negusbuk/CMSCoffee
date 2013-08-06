#ifndef CMSCOFFEEMAINWINDOW_H
#define CMSCOFFEEMAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QTableWidget>
#include <QToolBar>

#include <cmscoffeeusermodel.h>
#include <cmscoffeetickmodel.h>
#include <cmscoffeeaccountmodel.h>
#include <cmscoffeeaccountentrycompletionmodel.h>

class CMSCoffeeMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    CMSCoffeeMainWindow(QWidget *parent = 0);
    ~CMSCoffeeMainWindow();

protected:

    CMSCoffeeUserModel* userModel_;
    CMSCoffeeActiveUserModel* activeUserModel_;
    CMSCoffeeTickModel* tickModel_;
    CMSCoffeeAccountModel* accountModel_;
    CMSCoffeeAccountEntryCompletionModel* completionModel_;

    QToolBar* toolBar_;
    QTabWidget* tabWidget_;
};

#endif // CMSCOFFEEMAINWINDOW_H
