#ifndef CMSCOFFEEUSER_H
#define CMSCOFFEEUSER_H

#include <vector>
#include <map>

#include <QString>
#include <QDate>

#include <cmscoffeetickentry.h>
#include <cmscoffeeaccountentry.h>

class CMSCoffeeUser
{
public:
    explicit CMSCoffeeUser();
    CMSCoffeeUser(const QString& uuid,
                  const QString& name,
                  bool active);
    CMSCoffeeUser(const QString& name,
                  bool active);

    const QString& getUUID() const { return uuid_; }
    const QString& getName() const { return name_; }
    bool getActive() const { return active_; }

    void setUUID(const QString& uuid) { uuid_ = uuid; }
    void setName(const QString& name) { name_ = name; }
    void setActive(bool active) { active_ = active; }
    
    void clearTickEntries() { tickEntries_.clear(); }
    void addTickEntry(CMSCoffeeTickEntry* entry);
    const std::vector<CMSCoffeeTickEntry*>& getTickEntries() const { return tickEntries_; }
    CMSCoffeeTickEntry* getTickEntry(const QDate& date);
    int getTickCount() const;
    float getTickBalance() const;

    void clearAccountEntries() { accountEntries_.clear(); }
    void addAccountEntry(CMSCoffeeAccountEntry* entry) { accountEntries_.push_back(entry); }
    void removeAccountEntry(CMSCoffeeAccountEntry* entry);
    const std::vector<CMSCoffeeAccountEntry*>& getAccountEntries() const { return accountEntries_; }
    float getAccountBalance() const;

    float getBalance() const;

protected:

    QString                              uuid_;
    QString                              name_;
    bool                                 active_;
    std::vector<CMSCoffeeTickEntry*>     tickEntries_;
    std::map<QDate,CMSCoffeeTickEntry*>  tickEntriesByDate_;
    std::vector<CMSCoffeeAccountEntry*>  accountEntries_;
};

#endif // CMSCOFFEEUSER_H
