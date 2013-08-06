#ifndef CMSCOFFEEACCOUNTENTRY_H
#define CMSCOFFEEACCOUNTENTRY_H

#include <QString>
#include <QDate>

class CMSCoffeeUser;

class CMSCoffeeAccountEntry
{
public:
    explicit CMSCoffeeAccountEntry(const QString uuid,
                                   const QDate& date, float amount,
                                   CMSCoffeeUser* user = 0);

    const QString& getUUID() const { return uuid_; }
    const QDate& getDate() const { return date_; }
    float getAmount() const { return amount_; }
    CMSCoffeeUser* getUser() const { return user_; }

    void setDate(const QDate& date) { date_ = date; }
    void setUUID(const QString& uuid) { uuid_ = uuid; }
    void setAmount(float amount) { amount_ = amount; }
    void setUser(CMSCoffeeUser* user) { user_ = user; }

protected:

    QString uuid_;
    QDate date_;
    float amount_;
    CMSCoffeeUser* user_;
};

#endif // CMSCOFFEEACCOUNTENTRY_H
