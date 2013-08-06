#ifndef CMSCOFFEEACCOUNTENTRYCOMPLETIONMODEL_H
#define CMSCOFFEEACCOUNTENTRYCOMPLETIONMODEL_H

#include <set>

#include <QAbstractListModel>

#include <cmscoffeeusermodel.h>
#include <cmscoffeeaccountmodel.h>

class CMSCoffeeAccountEntryCompletionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CMSCoffeeAccountEntryCompletionModel(CMSCoffeeUserModel* userModel,
                                                  CMSCoffeeAccountModel* accountModel,
                                                  QObject *parent = 0);

    int rowCount(const QModelIndex &index = QModelIndex()) const ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    
public slots:
    
    void refresh();
    void addAccountEntry(const QDate&date, const QString&name, float amount);

protected:

    CMSCoffeeUserModel* userModel_;
    CMSCoffeeAccountModel* accountModel_;
    std::set<QString> entries_;
};

#endif // CMSCOFFEEACCOUNTENTRYCOMPLETIONMODEL_H
