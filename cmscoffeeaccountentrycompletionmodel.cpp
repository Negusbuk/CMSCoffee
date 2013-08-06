#include <iostream>

#include "cmscoffeeaccountentrycompletionmodel.h"

CMSCoffeeAccountEntryCompletionModel::CMSCoffeeAccountEntryCompletionModel(CMSCoffeeUserModel* userModel,
                                                                           CMSCoffeeAccountModel* accountModel,
                                                                           QObject *parent) :
    QAbstractListModel(parent),
    userModel_(userModel),
    accountModel_(accountModel)
{
    refresh();
}

void CMSCoffeeAccountEntryCompletionModel::refresh()
{
    for (std::vector<CMSCoffeeUser*>::const_iterator it=userModel_->getUsers().begin();
         it!=userModel_->getUsers().end();
         ++it) {
        entries_.insert((*it)->getName());
    }

    for (std::vector<CMSCoffeeAccountEntry*>::const_iterator it=accountModel_->getEntries().begin();
         it!=accountModel_->getEntries().end();
         ++it) {
        QString uuid = (*it)->getUUID();
        if ((*it)->getUser()==0)
            entries_.insert(uuid);
    }
}

void CMSCoffeeAccountEntryCompletionModel::addAccountEntry(const QDate&/*date*/, const QString&name, float /*amount*/)
{
    CMSCoffeeUser* user = userModel_->userByUUID(name);
    if (user)
        entries_.insert(user->getName());
    else
        entries_.insert(name);
}

int CMSCoffeeAccountEntryCompletionModel::rowCount(const QModelIndex &index) const
{
    return entries_.size();
}

QVariant CMSCoffeeAccountEntryCompletionModel::data(const QModelIndex &index, int role) const
{
    if ((role==Qt::EditRole || role==Qt::DisplayRole)&& index.column()==0) {
        std::set<QString>::iterator it = entries_.begin();
        std::advance(it, index.row());
        QString s = *it;
        return s;
    }

    return QVariant();
}
