#include <iostream>

#include <QUuid>

#include "cmscoffeeuser.h"

CMSCoffeeUser::CMSCoffeeUser() :
    uuid_(QUuid::createUuid().toString().toUpper())
{

}

CMSCoffeeUser::CMSCoffeeUser(const QString& uuid,
                             const QString& name,
                             bool active) :
    uuid_(uuid),
    name_(name),
    active_(active)
{

}

CMSCoffeeUser::CMSCoffeeUser(const QString& name,
                             bool active) :
    uuid_(QUuid::createUuid().toString().toUpper()),
    name_(name),
    active_(active)
{

}

void CMSCoffeeUser::addTickEntry(CMSCoffeeTickEntry* entry)
{
    tickEntries_.push_back(entry);
    tickEntriesByDate_[entry->getDate()] = entry;
}

CMSCoffeeTickEntry* CMSCoffeeUser::getTickEntry(const QDate& date)
{
    std::map<QDate,CMSCoffeeTickEntry*>::iterator it = tickEntriesByDate_.find(date);
    if (it==tickEntriesByDate_.end()) return 0;
    return it->second;
}

void CMSCoffeeUser::removeAccountEntry(CMSCoffeeAccountEntry* entry)
{
    std::vector<CMSCoffeeAccountEntry*>::iterator it;
    it = std::find(accountEntries_.begin(), accountEntries_.end(), entry);
    if (it!=accountEntries_.end()) accountEntries_.erase(it);
}

int CMSCoffeeUser::getTickCount() const
{
    int count = 0;
    for (std::vector<CMSCoffeeTickEntry*>::const_iterator it = tickEntries_.begin();
         it!=tickEntries_.end();
         ++it) {
        count += (*it)->getCount();
    }
    return count;
}

float CMSCoffeeUser::getTickBalance() const
{
    float balance = 0;
    for (std::vector<CMSCoffeeTickEntry*>::const_iterator it = tickEntries_.begin();
         it!=tickEntries_.end();
         ++it) {
        balance += (*it)->getCount() * (*it)->getPrice();
    }
    return balance;
}

float CMSCoffeeUser::getAccountBalance() const
{
    float balance = 0;
    for (std::vector<CMSCoffeeAccountEntry*>::const_iterator it = accountEntries_.begin();
         it!=accountEntries_.end();
         ++it) {
        balance += (*it)->getAmount();
    }
    return balance;
}

float CMSCoffeeUser::getBalance() const
{
    float balance = getAccountBalance() - getTickBalance();
    return balance;
}
