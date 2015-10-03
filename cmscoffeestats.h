#ifndef CMSCOFFEESTATS_H
#define CMSCOFFEESTATS_H

#include <QObject>

#include <cmscoffeeusermodel.h>
#include <cmscoffeetickmodel.h>
#include <cmscoffeeaccountmodel.h>

class CMSCoffeeStats : public QObject
{
    Q_OBJECT
public:
    explicit CMSCoffeeStats(CMSCoffeeUserModel* userModel,
                            CMSCoffeeTickModel* tickModel,
                            CMSCoffeeAccountModel* accountModel,
                            QObject *parent = 0);
    
signals:
    
public slots:
    
    void makeStats();

protected:

    CMSCoffeeUserModel* userModel_;
    CMSCoffeeTickModel* tickModel_;
    CMSCoffeeAccountModel* accountModel_;

    void makeAccountHistory(const QString& dir);
    void makeBalanceHistory(const QString& dir);
    void makeWeeklyStats(const QString& dir);
    void makeUserList(const QString& dir);
    void makeTickList(const QString& dir);
};

#endif // CMSCOFFEESTATS_H
